#include "jkn/config.h"
#if JKN_CONFIG_SOCKETS
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

    void addressGetHostIp(const IPAddress& _address, char* _hostName, size_t _len)
    {
        if (_address.m_type == IPAddressType::IPv4)
        {
            uint8_t a = _address.m_ipv4 & 0xff;
            uint8_t b = (_address.m_ipv4 >> 8) & 0xff;
            uint8_t c = (_address.m_ipv4 >> 16) & 0xff;
            uint8_t d = (_address.m_ipv4 >> 24) & 0xff;

            snprintf(_hostName, _len, "%d.%d.%d.%d", a, b, c, d);
        }
        else
        {
            // yada yada
            JKN_ASSERT(0, "not implemented");
        }
    }
}

#endif


