#pragma once

#include <windows.h>

namespace jkn
{
    struct Mutex
    {
        Mutex()
        {
            InitializeCriticalSection(&m_cs);
        }

        ~Mutex()
        {
            DeleteCriticalSection(&m_cs);
        }

        void lock()
        {
            EnterCriticalSection(&m_cs);
        }

        void unlock()
        {
            LeaveCriticalSection(&m_cs);
        }

        CRITICAL_SECTION m_cs;
    private:
        Mutex(const Mutex&);
        Mutex& operator=(const Mutex&);
    };

    struct ScopedMutex
    {
        ScopedMutex(Mutex& mutex) 
            : m_mutex(mutex)
        {
            m_mutex.lock();
        }

        ~ScopedMutex()
        {
            m_mutex.unlock();
        }

    private:
        Mutex& m_mutex;
    };
}