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

#include "startupmanager.h"
#include "serialmanager.h"

#include <QDebug>
#include <packet.h>

StartupManager::StartupManager(QObject *parent)
    : QObject(parent)
{
    connect(&SerialManager::instance(), &SerialManager::statusChanged, this, [this](SerialManager::Status status){
        switch (status) {
        case SerialManager::Status::Connected:
            retry();
            break;
        case SerialManager::Status::Unconnected:
            setStatus(Status::Failure);
            break;
        default:
            break;
        }
    });
    connect(&SerialManager::instance(), &SerialManager::packetReady,
            this, [this](const Packet &packet) {
        if (packet.fields.type != Packet::Type::BackendStatus)
            return;
        m_heartBeat.start();
        auto payload = packet.payload<BackendStatusPayload>();
        switch (payload.status){
        case BackendStatusPayload::Status::NoError:
            setStatus(Status::Checked);
            break;
        case BackendStatusPayload::Status::Error:
            setStatus(Status::Failure);
            break;
        default:
            setStatus(Status::Checking);
            break;
        }
    });
    m_heartBeat.setSingleShot(true);
    m_heartBeat.setInterval(2500);
    connect(&m_heartBeat, &QTimer::timeout, this, [this]{
        setStatus(Status::Failure);
    });
}

StartupManager::Status StartupManager::status() const
{
    return m_status;
}

void StartupManager::setStatus(Status status)
{
    if (m_status == status)
        return;
    m_status = status;
    emit statusChanged(status);
}

void StartupManager::retry()
{
    setStatus(Status::Checking);
#ifdef DUMMY_DATA
    QTimer::singleShot(2500 + qrand() % 2500, this, [this] {
        setStatus(qrand() % 2 ? Status::Checked : Status::Failure);
    });
#else
    m_heartBeat.stop();
    if (SerialManager::instance().status() == SerialManager::Status::Connected) {
        SerialManager::instance().writePacket(Packet{Packet::Type::CheckHealth});
        m_heartBeat.start();
    } else {
        SerialManager::instance().retry();
    }
#endif
}
