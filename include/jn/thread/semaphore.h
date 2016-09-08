#pragma once

#include <windows.h>
#include <stdint.h> // uint32_t

namespace jn
{
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

        bool wait(int32_t msecs = -1) const
        {
            return WAIT_OBJECT_0 == WaitForSingleObject(
                m_handle,
                /*(msecs == -1) ? INFINITE : */msecs
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
}