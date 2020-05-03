/*
    Copyright (C) 2020, BogDan Vatra <bogdan@kde.org>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "serialmanager.h"

#include <QSerialPortInfo>

#include <functional>

#include <packet.h>


class ScopeGuard
{
public:
    ScopeGuard(const std::function<void()> &func)
        : m_func(func)
    {}
    ~ScopeGuard()
    {
        m_func();
    }
private:
    std::function<void()> m_func;
};

SerialManager &SerialManager::instance()
{
    static SerialManager res;
    return res;
}

SerialManager::Status SerialManager::status() const
{
    return m_status;
}

void SerialManager::retry()
{
    if (status() == Status::Connected)
        return;
    setStatus(Status::Connecting);
    auto ports = QSerialPortInfo::availablePorts();
    for (const auto &port :ports) {
        m_serialPort->close();
        m_serialPort->setPort(port);
        if (!m_serialPort->open(QIODevice::ReadWrite))
            continue;
        if (m_serialPort->error() != QSerialPort::NoError)
            continue;
        if (!writePacket({Packet::Type::Hello, HelloPayload{HelloPayload::Type::Ping}}))
            continue;
        if (!m_serialPort->waitForBytesWritten(1000))
            continue;

        while(m_serialPort->waitForReadyRead()) {
            if (auto packet = readPacket()) {
                auto hello = packet->payload<HelloPayload>();
                if (hello.type == HelloPayload::Type::Pong &&
                        hello.protocol == HelloPayload::Protocol::V1) {
                    setStatus(Status::Connected);
                    return;
                }
            }
        }
    }
    setStatus(Status::Unconnected);
}

bool SerialManager::writePacket(const Packet &packet)
{
    if (status() == Status::Unconnected || !packet)
        return false;
    auto sz = m_serialPort->write(packet.buffer, packet.packetSize());
    return sz == qint64(packet.packetSize());
}

SerialManager::SerialManager()
    :QObject(nullptr)
{
    m_serialPort = std::make_unique<QSerialPort>();
    m_serialPort->setBaudRate(QSerialPort::Baud115200);
    connect(m_serialPort.get(), &QSerialPort::readyRead, this, &SerialManager::processAvailableBytes);
    connect(m_serialPort.get(), &QSerialPort::errorOccurred, this, [this] {
        if (m_status == Status::Connected)
            setStatus(Status::Unconnected);
    });
}

void SerialManager::setStatus(SerialManager::Status status)
{
    if (m_status == status)
        return;

    m_status = status;
    emit statusChanged(m_status);
}

void SerialManager::processAvailableBytes()
{
    if (m_status != Status::Connected)
        return;
    while(m_serialPort->bytesAvailable()) {
        auto packet = readPacket();
        if (packet && *packet)
            emit packetReady(*packet);
    }
}

Packet *SerialManager::readPacket()
{
    static Packet packet;
    static size_t pos = 0;
    while(m_serialPort->bytesAvailable()) {
        if (pos < packet.headerSize()) {
            if (pos < sizeof(uint32_t)) {
                pos += m_serialPort->read(packet.buffer, sizeof(uint32_t) - pos);
                if (pos == sizeof(uint32_t)) {
                    // check the magic
                    if (packet.fields.magic != Packet::MAGIC) {
                        packet.fields.magic <<= 8;
                        --pos;
                        continue;
                    }
                }
            }
            pos += m_serialPort->read(packet.buffer + pos, Packet::headerSize());
            if (pos < Packet::headerSize())
                continue;

            // It's the packet size sane?
            if (!packet.isSane()) {
                pos = 0;
                continue;
            }
        }
        pos += m_serialPort->read(packet.buffer + pos, packet.packetSize() - pos);

        // Do we have a full packet?
        if (pos == packet.packetSize()) {
            pos = 0;
            return packet ? &packet : nullptr;
        }
    }
    return nullptr;
}
