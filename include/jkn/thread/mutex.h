#pragma once

#include "../platform.h"
#if JKN_PLATFORM_WINDOWS
#   include <windows.h>
#elif JKN_PLATFORM_LINUX
#   include <pthread.h>
#endif

namespace jkn
{
#if JKN_PLATFORM_WINDOWS
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

#elif JKN_PLATFORM_LINUX
    
    struct Mutex
    {
        Mutex()
        {
            pthread_mutexattr_t attr;
            int32_t result = pthread_mutexattr_init(&attr);
            JKN_ASSERT(result == 0, "pthread_mutexattr_init failed with code %d", result);

            result = pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
            JKN_ASSERT(result == 0, "pthread_mutexattr_settype failed with code %d", result);

            result = pthread_mutex_init(&m_handle, &attr);
            JKN_ASSERT(result == 0, "pthread_mutex_init failed with code %d", result);
            (void)result;
        }

        ~Mutex()
        {
            int32_t result = pthread_mutex_destroy(&m_handle);
            JKN_ASSERT(result == 0, "pthread_mutex_destroy failed with code %d", result);
            (void)result;
        }

        void lock()
        {
            int32_t result = pthread_mutex_lock(&m_handle);
            JKN_ASSERT(result == 0, "pthread_mutex_lock failed with code %d", result);
            (void)result;
        }

        void unlock()
        {
            int32_t result = pthread_mutex_unlock(&m_handle);
            JKN_ASSERT(result == 0, "pthread_mutex_unlock failed with code %d", result);
            (void)result;
        }

        pthread_mutex_t m_handle;
    private:
        Mutex(const Mutex&);
        Mutex& operator=(const Mutex&);
    };

#endif

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
