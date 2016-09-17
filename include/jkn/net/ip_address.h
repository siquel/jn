#pragma once

#include "jkn/config.h"
#if JKN_CONFIG_SOCKETS

#include <stdint.h>

namespace jkn
{
    struct IPAddressType
    {
        enum Enum
        {
            None,
            IPv4,
            IPv6
        };
    };

    struct IPAddress
    {
        IPAddressType::Enum m_type;

        // Stored in network byte order (big endian)
        union
        {
            uint32_t m_ipv4;
            uint16_t m_ipv6[8];
        };

        uint16_t m_port;
    };
}

#endif