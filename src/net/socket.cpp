
#include "jkn/config.h"

#if JKN_CONFIG_SOCKETS

#include "jkn/platform.h"
#if JKN_PLATFORM_WINDOWS
#   define NOMINMAX
#   define _WINSOCK_DEPRECATED_NO_WARNINGS
#   include <winsock2.h>
#   include <ws2tcpip.h>
#   include <ws2ipdef.h>
#endif

#include "jkn/net/socket.h"
#include "jkn/net/ip_address.h"
#include "jkn/error.h"
#include <stdio.h> // snprintf
#include "jkn/debug.h" // debugOutput

namespace jkn
{
    UDPSocket::UDPSocket(const IPAddress& _address) :
        m_error(SocketError::NoError)
    {
        JKN_ASSERT(_address.m_type != IPAddressType::None, "Invalid address type");

        m_socket = socket(
            (_address.m_type == IPAddressType::IPv4) ? AF_INET : AF_INET6,
            SOCK_DGRAM,
            IPPROTO_UDP
            );

#if JKN_PLATFORM_WINDOWS
        if (m_socket == INVALID_SOCKET)
        {
            m_error = SocketError::CreateFailed;
            char buf[128] = {};
            snprintf(buf, sizeof(buf), "Socket creation failed: WSAGetLastError() = %d\n", WSAGetLastError());
            jkn::debugOutput(buf);
            return;
        }
#endif

        if (_address.m_type == IPAddressType::IPv4)
        {
            sockaddr_in sockAddress;
            sockAddress.sin_family = AF_INET;
            sockAddress.sin_port = htons(_address.m_port);
            sockAddress.sin_addr.s_addr = _address.m_ipv4;

            if (::bind(m_socket, (struct sockaddr*)&sockAddress, sizeof(sockAddress)) == SOCKET_ERROR)
            {
                m_error = SocketError::BindFailed;
                return;
            }
        }
        else if (_address.m_type == IPAddressType::IPv6)
        {
            sockaddr_in6 sockAddress;
            sockAddress.sin6_family = AF_INET6;
            memcpy(&sockAddress.sin6_addr, _address.m_ipv6, sizeof(_address.m_ipv6));
            sockAddress.sin6_port = htons(_address.m_port);

            if (::bind(m_socket, (struct sockaddr*)&sockAddress, sizeof(sockAddress)) == SOCKET_ERROR)
            {
                m_error = SocketError::BindFailed;
                return;
            }
        }

#if JKN_PLATFORM_WINDOWS
        DWORD iMode = 1; // If iMode != 0, non-blocking mode is enabled.
        if (::ioctlsocket(m_socket, FIONBIO, &iMode) != NO_ERROR)
        {
            m_error = SocketError::SetToNonBlockingFailed;
            return;
        }
#endif
    }

    UDPSocket::~UDPSocket()
    {
        if (m_socket == 0) return;

#if JKN_PLATFORM_WINDOWS
        ::closesocket(m_socket);
#endif
        m_socket = 0;
    }
}

#endif