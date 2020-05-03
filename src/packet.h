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

#pragma once

#include <stdint.h>
#include <stddef.h>

/**
 * @brief The Packet struct. This struct represents the data format used
 * to send and receive data over serial port.
 */
struct Packet
{
    enum {
        MAX_PAYLOAD_SIZE = 64,
        MAGIC = 0x0a724941 // AIr\n
    };

    enum class Type : uint8_t
    {
        Hello = 0,
        CheckHealth,
        FrontEndStatus,
        BackendStatus,
        SensorsData,
        Nofitication,
        Heartbeat,
        Invalid = 0xff
    };

    union {
        struct __attribute__((__packed__)) {
            uint32_t magic;
            uint16_t crc;
            uint8_t size;
            Type type;
        } fields;
        char buffer[MAX_PAYLOAD_SIZE];
    };
    static_assert (sizeof(fields) % 8 == 0, "Fileds must me 8 bytes alligned");
    /**
     * @brief Packet creates a new Packet structure.
     * @param type the packet type, default it's Invalid
     */
    Packet(Type type = Type::Invalid);

    template<typename T>
    Packet(Type type, const T &val)
        : Packet(type)
    {
        setPayload(val);
    }

    /**
     * @brief Creates a new Packet struct and sets the \a text as it's payload
     */
    Packet(Type type, const char *text);

    /**
     * @brief Create a new Packet struct with the given \a type, \a data and \a size
     */
    Packet(Type type, const char *data, size_t size);

    /**
     * @brief operator bool useful to check if the packet it's valid.
     * It checks the magic, the crc and the packageSize
     */
    inline operator bool() const
    {
        return fields.magic == MAGIC &&
                packetSize() <= MAX_PAYLOAD_SIZE &&
                checkCrc();
    }

    /**
     * @brief isSane
     * @return true if the magic and packetSize() are valid
     */
    inline bool isSane() const
    {
        return fields.magic == MAGIC && packetSize() <= MAX_PAYLOAD_SIZE;
    }

    /**
     * @brief setPayload sets the packet payload
     * @return true if success
     */
    bool setPayload(const char *txt);
    bool setPayload(const char *data, size_t size);

    template<typename T>
    bool setPayload(const T &val) {
        return setPayload(reinterpret_cast<const char*>(&val), sizeof(T));
    }

    /**
     * @brief payload
     * @return the packet payload
     */
    inline char *payload() { return buffer + headerSize(); }
    inline const char *payload() const { return buffer + headerSize(); }
    template<typename T>
    inline const T &payload() const { return *reinterpret_cast<const T*>(payload()); }

    /**
     * @brief checkCrc
     * @return true if the crc is valid
     */
    bool checkCrc() const;


    /**
     * @brief computeCrc computes the package crc. It's useful when
     * the we set the payload manually
     */
    void computeCrc();

    /**
     * @brief packetSize
     * @return the packet size (header + payload)
     */
    inline size_t packetSize() const { return fields.size + sizeof(fields); }

    /**
     * @brief headerSize
     * @return the header size
     */
    static constexpr size_t headerSize() { return sizeof(fields); }

private:
    uint16_t checksum() const;
};

struct HelloPayload
{
    enum class Type : uint8_t {
        Ping,
        Pong
    };
    enum class Protocol : uint8_t {
        V1
    };
    HelloPayload(Type _type, Protocol _protocol = Protocol::V1)
        : type(_type)
        , protocol(_protocol)
    {}
    Type type;
    Protocol protocol;
}__attribute__((__packed__));

struct BackendStatusPayload {
    enum class Status : uint8_t {
        NoError,
        Checking,
        Error
    };
    BackendStatusPayload(Status _status)
        : status(_status)
    {}
    Status status;
}__attribute__((__packed__));

struct SensorsDataPayload {
    float pressure;
    float flow;
    float  volume;
}__attribute__((__packed__));
