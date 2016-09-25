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

    struct UDPSocket
    {
#if JKN_PLATFORM_WINDOWS
        size_t m_socket;
#elif JKN_PLATFORM_POSIX
        int32_t m_socket;
#endif
        SocketError::Enum m_error;
    };

    /// \brief Create a socket
    /// 
    /// \param[out] _socket Where to create the socket
    /// \param[in] _address Which IP type socket to craete
    /// \return 0 if succeeded, < 0 if failed
    int32_t socket(UDPSocket& _socket, const IPAddress& _address);

    /// \brief Bind socket to address
    /// 
    /// \param[in,out] _socket The socket to bind
    /// \param[out] _address The address where to bind socket
    /// \return 0 if successfull, < 0 if failed
    int32_t bind(UDPSocket& _socket, const IPAddress& _address);

    /// \brief Set socket I/O mode to non-blocking
    /// 
    /// \param[in,out]
    int32_t setToNonBlocking(UDPSocket& _socket);

    /// \brief Close socket
    ///
    /// \param[in] _socket The socket to close
    void close(UDPSocket& _socket);

    /// \brief Send a message to specific address
    /// 
    /// \param[in] _socket The socket
    /// \param[in] _to The address of remote peer
    /// \param[in] _data Pointer to data buffer
    /// \param[in] _dataSize How many bytes to send from buffer
    /// \return true if all bytes were sent
    bool sendto(UDPSocket& _socket, const IPAddress& _to, const void* _data, int32_t _dataSize);

    /// \brief Receive a message from a socket
    /// 
    /// \param[in] _socket The socket
    /// \param[out] _data Buffer where to read
    /// \param[in] _dataSize Size of the buffer in bytes
    /// \param[out] _from The origin of the packet
    /// \return How many bytes were read
    int32_t receive(UDPSocket& _socket, void* _data, int32_t _dataSize, IPAddress& _from);
}

#endif