
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
#   include <netinet/in.h> // s_addr
#   include <fcntl.h> //fcntl
#   include <sys/socket.h> // socket
#endif


#include "jkn/net/socket.h"
#include "jkn/net/ip_address.h"
#include "jkn/error.h"
#include <string.h> // memcpy
#include <stdio.h> // snprintf
#include "jkn/debug.h" // debugOutput

namespace jkn
{
    int32_t socket(UDPSocket& _socket, const IPAddress& _address)
    {
        _socket.m_socket = ::socket(
            (_address.m_type == IPAddressType::IPv4) ? AF_INET : AF_INET6,
            SOCK_DGRAM,
            IPPROTO_UDP
            );

#if JKN_PLATFORM_WINDOWS
        if (_socket.m_socket == INVALID_SOCKET)
        {
            _socket.m_socket = 0;
            char buf[128] = {};
            snprintf(buf, sizeof(buf), "Socket creation failed: WSAGetLastError() = %d\n", WSAGetLastError());
            jkn::debugOutput(buf);
            return -1;
        }
#elif JKN_PLATFORM_LINUX
#   error "socket creation error checking not implemented"
#endif
        return 0;
    }

    int32_t bind(UDPSocket& _socket, const IPAddress& _address)
    {
        if (_address.m_type == IPAddressType::IPv4)
        {
            sockaddr_in sockAddress;
            sockAddress.sin_family = AF_INET;
            sockAddress.sin_port = htons(_address.m_port);
            sockAddress.sin_addr.s_addr = _address.m_ipv4;

            if (::bind(_socket.m_socket, (struct sockaddr*)&sockAddress, sizeof(sockAddress)) < 0) // SOCKET_ERROR = -1
            {
                return -1;
            }
        }
        else if (_address.m_type == IPAddressType::IPv6)
        {
            sockaddr_in6 sockAddress;
            sockAddress.sin6_family = AF_INET6;
            memcpy(&sockAddress.sin6_addr, _address.m_ipv6, sizeof(_address.m_ipv6));
            sockAddress.sin6_port = htons(_address.m_port);

            if (::bind(_socket.m_socket, (struct sockaddr*)&sockAddress, sizeof(sockAddress)) < 0) // SOCKET_ERROR = -1
            {
                return -1;
            }

            // force ipv6
            int yes = 1;
            if (::setsockopt(_socket.m_socket, IPPROTO_IPV6, IPV6_V6ONLY, (char*)&yes, sizeof(yes)) != 0)
            {
                return -1;
            }
        }
        else // IPAddressType::None
        {
            return -1;
        }

        return 0;
    }

    int32_t setToNonBlocking(UDPSocket& _socket)
    {
#if JKN_PLATFORM_WINDOWS
        DWORD iMode = 1; // If iMode != 0, non-blocking mode is enabled.
        if (::ioctlsocket(_socket.m_socket, FIONBIO, &iMode) != NO_ERROR)
        {
            return -1;
        }
#elif JKN_PLATFORM_LINUX
        int32_t nonBlocking = 1;
        if (::fcntl(_socket.m_socket, F_SETFL, O_NONBLOCK, nonBlocking) == -1)
        {
            return -1;
        }
#endif
        return 0;
    }

    void close(UDPSocket& _socket)
    {
        if (_socket.m_socket == 0) return;

#if JKN_PLATFORM_WINDOWS
        ::closesocket(_socket.m_socket);
#elif JKN_PLATFORM_LINUX
        ::close(_socket.m_socket);
#endif
        _socket.m_socket = 0;
    }

    int32_t receive(UDPSocket& _socket, void* _buffer, int32_t _bufferLength, IPAddress& _from)
    {
        // If you want to write code that supports multiple protocols, use sockaddr_storage (SO)
        // http://stackoverflow.com/questions/18995361/can-recvfrom-function-from-socket-extract-the-sender-ip-address

        sockaddr_in sockaddrFrom;
        socklen_t length = sizeof(sockaddrFrom);

        int32_t result = ::recvfrom(_socket.m_socket, (char*)_buffer, _bufferLength, 0 /* flags */, (struct sockaddr*) &sockaddrFrom, &length);

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
#elif JKN_PLATFORM_LINUX
#   error "error checking not implemented"
#endif
        JKN_ASSERT(result >= 0, "Invalid result from recvfrom()");

        if (sockaddrFrom.sin_family == AF_INET)
        {
            struct sockaddr_in* ipv4 = (struct sockaddr_in*) &sockaddrFrom;
            _from.m_type = IPAddressType::IPv4;
            _from.m_ipv4 = ipv4->sin_addr.s_addr;
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

    bool sendto(UDPSocket& _socket, const IPAddress& _to, const void* _data, int32_t _dataSize)
    {
        JKN_ASSERT(_dataSize > 0, "_bytes must be higher than 0");
        JKN_ASSERT(_to.m_type != IPAddressType::None, "Address must be ipv4 or ipv6 address");

        if (_to.m_type == IPAddressType::IPv4)
        {
            sockaddr_in sockAddr;
            memset(&sockAddr, 0, sizeof(sockAddr));
            sockAddr.sin_family = AF_INET;
            sockAddr.sin_port = htons(_to.m_port);
            sockAddr.sin_addr.s_addr = _to.m_ipv4;
            int32_t sentBytes = sendto(_socket.m_socket, (const char*)_data, _dataSize, 0 /* flags */, (struct sockaddr*)&sockAddr, sizeof(sockaddr_in));

            return sentBytes == _dataSize;
        }

        return false;
    }
}

#endif
