#pragma once

#include "jkn/config.h"
#if JKN_CONFIG_SOCKETS

#include <stdint.h> // uintX_t
#include <stdlib.h> // size_t
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

    void addressGetHostIp(const IPAddress& _address, char* _hostName, size_t _len);

    /// \param _a first octet host byte order
    /// \param _b second octet host byte order
    /// \param _c third octet host byte order
    /// \param _d fourth octet host byte order
    void addressSetHost(IPAddress& _address, uint8_t _a, uint8_t _b, uint8_t _c, uint8_t _d);

    /// \param _ipv4 ipv4 address in host byte order
    void addressSetHost(IPAddress& _address, uint32_t _ipv4);

    /// host byte order
    void addressSetHost(IPAddress& _address, uint16_t _a, uint16_t _b, uint16_t _c, uint16_t _d,
        uint16_t _e, uint16_t _f, uint16_t _g, uint16_t _h);
}

#endif
