#pragma once

#include "../config.h"
#include "../platform.h"
#include <stdint.h> // int32_t

#if JKN_CONFIG_SOCKETS

namespace jkn
{
    struct IPAddress;

    struct SocketError
    {
        enum Enum
        {
            NoError,
            CreateFailed,
            BindFailed,
            SetToNonBlockingFailed
        };
    };

    class UDPSocket
    {
    public:
        UDPSocket(const IPAddress& _address);

        ~UDPSocket();

        bool send(const IPAddress& _to, const void* _data, size_t _bytes);

        /// \brief Receive a message from a socket
        /// 
        /// \param[out] _data Buffer where to read
        /// \param[in] _bufferLength Length of _data
        /// \param[out] _from The origin of the packet
        /// \return The bytes read
        int32_t receive(void* _data, int32_t _bufferLength, IPAddress& _from);

    private:
#if JKN_PLATFORM_WINDOWS
        size_t m_socket;
#elif JKN_PLATFORM_POSIX
        int32_t m_socket;
#endif
        SocketError::Enum m_error;
    };
}

#endif