#pragma once

#include "../config.h"
#include "../platform.h"
#include <stdint.h> // int32_t

#if JKN_CONFIG_SOCKETS

namespace jkn
{
    struct IPAddress;

    class UDPSocket
    {
    public:
        UDPSocket(const IPAddress& _address);

        ~UDPSocket();

        bool send(const IPAddress& _to, const void* _data, size_t _bytes);

        int32_t receive();

    private:
#if JKN_PLATFORM_WINDOWS
        size_t m_socket;
#elif JKN_PLATFORM_POSIX
        int32_t m_socket;
#endif
    };
}

#endif