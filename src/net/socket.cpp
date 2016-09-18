
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

    int32_t UDPSocket::receive(void* _data, int32_t _bufferLength, IPAddress& _from)
    {
        // If you want to write code that supports multiple protocols, use sockaddr_storage (SO)
        // http://stackoverflow.com/questions/18995361/can-recvfrom-function-from-socket-extract-the-sender-ip-address

        sockaddr_in sockaddrFrom;
        socklen_t length = sizeof(sockaddrFrom);

        int32_t result = ::recvfrom(m_socket, (char*)_data, _bufferLength, 0 /* flags */, (struct sockaddr*) &sockaddrFrom, &length);

#if JKN_PLATFORM_WINDOWS
        
        if (result == SOCKET_ERROR)
        {
            int32_t error = WSAGetLastError();

            if (error == WSAEWOULDBLOCK)
            {
                return 0;
            }

            // TODO trace

            return 0;
        }
#endif
        JKN_ASSERT(result >= 0, "Invalid result from recvfrom()");

        if (sockaddrFrom.sin_family == AF_INET)
        {
            struct sockaddr_in* ipv4 = (struct sockaddr_in*) &sockaddrFrom;
            _from.m_type = IPAddressType::IPv4;
            _from.m_ipv4 = ipv4->sin_addr.S_un.S_addr;
            _from.m_port = ntohs(ipv4->sin_port); // port is host byte
        }
        else if (sockaddrFrom.sin_family == AF_INET6)
        {
            struct sockaddr_in6* ipv6 = (struct sockaddr_in6*) &sockaddrFrom;
            _from.m_type = IPAddressType::IPv6;
            memcpy(_from.m_ipv6, &ipv6->sin6_addr, sizeof(_from.m_ipv6));
            _from.m_port = ntohs(ipv6->sin6_port); // port is host byte
        }

        // return bytes read
        return result;
    }

    bool UDPSocket::send(const IPAddress& _to, const void* _data, int32_t _bytes)
    {
        JKN_ASSERT(_bytes > 0, "_bytes must be higher than 0");
        JKN_ASSERT(m_error == 0, "Socket has error");
        JKN_ASSERT(_to.m_type != IPAddressType::None, "Address must be ipv4 or ipv6 address");

        if (_to.m_type == IPAddressType::IPv4)
        {
            sockaddr_in sockAddr;
            memset(&sockAddr, 0, sizeof(sockAddr));
            sockAddr.sin_family = AF_INET;
            sockAddr.sin_port = htons(_to.m_port);
            sockAddr.sin_addr.S_un.S_addr = _to.m_ipv4;
            int32_t sentBytes = sendto(m_socket, (char*)_data, _bytes, 0 /* flags */, (struct sockaddr*)&sockAddr, sizeof(sockaddr_in));

            return sentBytes == _bytes;
        }
        
        return false;
    }
}

#endif