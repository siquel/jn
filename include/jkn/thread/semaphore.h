#pragma once

#include "../platform.h"
#if JKN_PLATFORM_WINDOWS
#   include <windows.h>
#elif JKN_PLATFORM_LINUX
#   include <errno.h> // errno
#   include <semaphore.h> //sem
#   include <time.h> // timespec
#endif
#include <stdint.h> // uint32_t

namespace jkn
{
#if JKN_PLATFORM_WINDOWS
    struct Semaphore
    {
        Semaphore() : 
            m_handle(INVALID_HANDLE_VALUE)
        {
            m_handle = CreateSemaphoreA(
                NULL,
                0, // init
                LONG_MAX, // max
                NULL
                );
        }

        ~Semaphore()
        {
            CloseHandle(m_handle);
        }

        bool wait(int32_t _msecs = -1) const
        {
            return WAIT_OBJECT_0 == WaitForSingleObject(
                m_handle,
                (_msecs < 0) ? INFINITE : _msecs
                );
        }

        void post(uint32_t _count = 1) const
        {
            ReleaseSemaphore(m_handle, _count, NULL);
        }

        HANDLE m_handle;
    private:
        Semaphore(const Semaphore&); 
        Semaphore& operator=(const Semaphore&);
    };
#elif JKN_PLATFORM_LINUX
    struct Semaphore
    {
        Semaphore() 
        {
            int32_t result = sem_init(&m_handle, 0, 0);
            // TODO assert res == 0
            (void) result;
        }

        ~Semaphore()
        {
            int32_t result = sem_destroy(&m_handle);
            // TODO assert res == 0
            (void)result;
        }

        bool wait(int32_t _msecs = -1) 
        {
            (void)_msecs;
            return false;
        }

        void post(uint32_t _count = 1) 
        {
            int32_t result;
            for (uint32_t i = 0; i < _count; ++i)
            {
                result = sem_post(&m_handle);
                // TODO assert res == 0
            }
            (void)result;
        }
    private:
        sem_t m_handle;

        Semaphore(const Semaphore&); 
        Semaphore& operator=(const Semaphore&);
    };

#endif
}
