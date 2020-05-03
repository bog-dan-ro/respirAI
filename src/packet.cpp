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

#include "packet.h"

#include <string.h>

Packet::Packet(Packet::Type type)
{
    fields.magic = MAGIC;
    fields.type = type;
    if (type != Type::Invalid) {
        fields.size = 0;
        computeCrc();
    }
}

Packet::Packet(Packet::Type type, const char *text)
    :Packet(type)
{
    setPayload(text);
}

Packet::Packet(Packet::Type type, const char *data, size_t size)
    :Packet(type)
{
    setPayload(data, size);
}

bool Packet::setPayload(const char *txt)
{
    return setPayload(txt, strlen(txt));
}

bool Packet::setPayload(const char *data, size_t size)
{
    fields.size = size;
    if (packetSize() > sizeof(Packet)) {
        fields.size = 0;
        return false;
    }
    memcpy(buffer + headerSize(), data, size);
    computeCrc();
    return true;
}

bool Packet::checkCrc() const
{
    return fields.crc == checksum();
}

void Packet::computeCrc()
{
    fields.crc = checksum();
}

uint16_t Packet::checksum() const
{
    uint16_t crc = 0xf00d;
    auto sz = fields.size;
    auto p = payload();
    while(sz--) {
        crc ^= *p++;
        crc <<= 1;
    }
    return crc;
}
