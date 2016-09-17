#pragma once

#include "../config.h"
#include "../platform.h"
#include <stdint.h> // int32_t

#if JKN_CONFIG_SOCKETS

namespace jkn
{
    struct IPAddress;

    class Socket
    {
    public:
        Socket(const IPAddress& _address);

        ~Socket();

        bool send(const IPAddress& _to, const void* _data, size_t _bytes);

        int32_t receive();
    };
}

#endif