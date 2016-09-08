#pragma once

#include <stdint.h>

#include <windows.h>
#include "semaphore.h"

namespace jn
{
    struct Thread
    {
        typedef int32_t(*ThreadFunc)(void* data);

        Thread() :
            m_func(NULL),
            m_userData(NULL),
            m_handle(INVALID_HANDLE_VALUE),
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
            m_func = _func;
            m_userData = _userData;

            m_handle = CreateThread(
                NULL,
                _stackSize,
                Thread::threadProc,
                this,
                0,
                NULL);

            m_running = true;

            m_semaphore.wait();
        }

        void join()
        {
            WaitForSingleObject(m_handle, INFINITE);
            GetExitCodeThread(m_handle, (DWORD*)&m_exitCode);
            CloseHandle(m_handle);
            m_handle = INVALID_HANDLE_VALUE;

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

        static DWORD WINAPI threadProc(void* arg)
        {
            int32_t result = ((Thread*)arg)->run();
            return result;
        }

        ThreadFunc m_func;
        void* m_userData;

        HANDLE m_handle;
        int32_t m_exitCode;

        Semaphore m_semaphore;

        bool m_running;

        

        Thread(const Thread&);
        Thread& operator=(const Thread&);
    };
}