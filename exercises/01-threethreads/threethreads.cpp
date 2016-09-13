#include <jkn/jkn.h>
#include <jkn/thread/thread.h>
#include <jkn/thread/mutex.h>
#include <jkn/os.h>
#include <jkn/platform.h>
#include <stdio.h> // printf
/*
Implement a code that starts three threads that prints "Hello world"
from thread X", where X=thread id. Take care of protecting critical
sections!
*/

static bool s_exit = false;

jkn::Mutex stdoutMutex;

int32_t threadProc(void* data)
{
    while (!s_exit)
    {
        stdoutMutex.lock();
        printf("Hello world from thread %d\n", *(uint32_t*)data);
        stdoutMutex.unlock();
        jkn::sleep(100);
    }
    return 0;
}
#if JKN_PLATFORM_WINDOWS
BOOL WINAPI sigHandler(DWORD sig)
{
    if (sig == CTRL_C_EVENT) s_exit = true;

    return TRUE;
}
#endif

int main(int argc, char** argv)
{
#if JKN_PLATFORM_WINDOWS
    SetConsoleCtrlHandler(sigHandler, TRUE);
#endif

    const uint32_t numThreads = 3;
    jkn::Thread threads[numThreads];
    uint32_t ids[numThreads] = {};
    for (uint32_t i = 0; i < numThreads; ++i)
    {
        ids[i] = i;
        threads[i].start(threadProc, &ids[i]);
    }

    for (uint32_t i = 0; i < numThreads; ++i)
    {
        threads[i].join();
    }

    return 0;
}
