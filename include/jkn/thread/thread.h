#pragma once

#include "../platform.h"
#include <stdint.h>

#if JKN_PLATFORM_WINDOWS
#   include <windows.h>
#elif JKN_PLATFORM_LINUX
#   include <pthread.h>
#endif

#include "jkn/error.h" // JKN_ASSERT
#include "semaphore.h"


namespace jkn
{
    struct Thread
    {
        typedef int32_t(*ThreadFunc)(void* data);

        Thread() :
            m_func(NULL),
            m_userData(NULL),
#if JKN_PLATFORM_WINDOWS
            m_handle(INVALID_HANDLE_VALUE),
#elif JKN_PLATFORM_LINUX
            m_handle(0),
#endif
            m_exitCode(0),
            m_running(false)
        {

        }

        ~Thread()
        {
            if (m_running)
            {
                join();
            }
        }

        void start(ThreadFunc _func, void* _userData = NULL, uint32_t _stackSize = 0)
        {
            JKN_ASSERT(!m_running, "Already running");

            m_func = _func;
            m_userData = _userData;

#if JKN_PLATFORM_WINDOWS
            m_handle = CreateThread(
                NULL,
                _stackSize,
                Thread::threadProc,
                this,
                0,
                NULL);
#elif JKN_PLATFORM_LINUX
            pthread_attr_t attr;
            int32_t result = pthread_attr_init(&attr);
            JKN_ASSERT(result == 0, "pthread_attr_init failed with code %d", result);

            if (_stackSize != 0)
            {
                result = pthread_attr_setstacksize(&attr, _stackSize);
                JKN_ASSERT(result == 0, "pthread_attr_setstacksize failed with code %d", result);
            }

            result = pthread_create(&m_handle, &attr, Thread::threadProc, this);
            JKN_ASSERT(result == 0, "pthread_create failed with code %d", result);
            (void)result;
#endif

            m_running = true;

            m_semaphore.wait();
        }

        void join()
        {
            JKN_ASSERT(m_running, "Not running");
#if JKN_PLATFORM_WINDOWS
            WaitForSingleObject(m_handle, INFINITE);
            GetExitCodeThread(m_handle, (DWORD*)&m_exitCode);
            CloseHandle(m_handle);
            m_handle = INVALID_HANDLE_VALUE;
#elif JKN_PLATFORM_LINUX
            union 
            {
                void* ptr;
                int32_t i;
            } cast;

            int32_t result = pthread_join(m_handle, &cast.ptr);
            JKN_ASSERT(result == 0, "pthread_join_failed with code %d", result);

            m_exitCode = cast.i;
            m_handle = 0;

            (void)result;
#endif
            m_running = false;
        }

        bool isRunning() const
        {
            return m_running;
        }

        int32_t getExitCode() const
        {
            return m_exitCode;
        }
    private:

        int32_t run()
        {
            m_semaphore.post();
            int32_t result = m_func(m_userData);
            return result;
        }
#if JKN_PLATFORM_WINDOWS
        static DWORD WINAPI threadProc(void* arg)
        {
            int32_t result = ((Thread*)arg)->run();
            return result;
        }
#elif JKN_PLATFORM_LINUX
        static void* threadProc(void* arg)
        {
            union
            {
                void* ptr;
                int32_t i;
            } cast;
            cast.i = ((Thread*)arg)->run();
            return cast.ptr;
        }
#endif

        ThreadFunc m_func;
        void* m_userData;

#if JKN_PLATFORM_WINDOWS
        HANDLE m_handle;
#elif JKN_PLATFORM_LINUX
        pthread_t m_handle;
#endif
        int32_t m_exitCode;

        Semaphore m_semaphore;

        bool m_running;
        
        Thread(const Thread&);
        Thread& operator=(const Thread&);
    };
}
