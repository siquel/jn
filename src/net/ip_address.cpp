#include "jkn/config.h"
#if JKN_CONFIG_SOCKETS

#include "jkn/platform.h"
#if JKN_PLATFORM_WINDOWS
#   define NOMINMAX
#   define _WINSOCK_DEPRECATED_NO_WARNINGS
#   include <winsock2.h>
#   include <ws2tcpip.h>
#   include <ws2ipdef.h>
#elif JKN_PLATFORM_LINUX
#   include <arpa/inet.h> // htons, htonl

#endif

#include "jkn/net/ip_address.h"
#include <stdio.h> // snprintf
#include "jkn/error.h" // JKN_ASSERT

namespace jkn
{
    void addressSetHost(IPAddress& _address, uint8_t _a, uint8_t _b, uint8_t _c, uint8_t _d)
    {
        _address.m_type = IPAddressType::IPv4;
        _address.m_ipv4 = uint32_t(_a) | (uint32_t(_b) << 8) | (uint32_t(_c) << 16) | (uint32_t(_d) << 24);
    }

    void addressSetHost(IPAddress& _address, uint32_t _ipv4)
    {
        _address.m_type = IPAddressType::IPv4;
        _address.m_ipv4 = htonl(_ipv4);
    }

    void addressSetHost(IPAddress& _address, uint16_t _a, uint16_t _b, uint16_t _c, uint16_t _d, uint16_t _e, uint16_t _f, uint16_t _g, uint16_t _h)
    {
        _address.m_type = IPAddressType::IPv6;
        _address.m_ipv6[0] = htons(_a);
        _address.m_ipv6[1] = htons(_b);
        _address.m_ipv6[2] = htons(_c);
        _address.m_ipv6[3] = htons(_d);
        _address.m_ipv6[4] = htons(_e);
        _address.m_ipv6[5] = htons(_f);
        _address.m_ipv6[6] = htons(_g);
        _address.m_ipv6[7] = htons(_h);
    }

    void addressGetHostIp(const IPAddress& _address, char* _hostName, size_t _len)
    {
        if (_address.m_type == IPAddressType::IPv4)
        {
            /*uint8_t a = _address.m_ipv4 & 0xff;
            uint8_t b = (_address.m_ipv4 >> 8) & 0xff;
            uint8_t c = (_address.m_ipv4 >> 16) & 0xff;
            uint8_t d = (_address.m_ipv4 >> 24) & 0xff;*/

            inet_ntop(AF_INET, (void*)&_address.m_ipv4, _hostName, _len);
        }
        else if (_address.m_type == IPAddressType::IPv6)
        {
            inet_ntop(AF_INET6, (void*)&_address.m_ipv6, _hostName, _len);
        }
    }
}

#endif


