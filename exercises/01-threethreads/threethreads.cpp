#include <jn/jn.h>
#include <jn/thread/thread.h>
#include <jn/thread/mutex.h>
#include <stdio.h>
/*
Implement a code that starts three threads that prints "Hello world"
from thread X", where X=thread id. Take care of protecting critical
sections!
*/

static bool s_exit = false;

jn::Mutex stdoutMutex;

int32_t threadProc(void* data)
{
    while (!s_exit)
    {
        stdoutMutex.lock();
        printf("Hello world from thread %d\n", *(uint32_t*)data);
        stdoutMutex.unlock();
        ::Sleep(100);
    }
    return 0;
}

BOOL WINAPI sigHandler(DWORD sig)
{
    if (sig == CTRL_C_EVENT) s_exit = true;

    return TRUE;
}

int main(int argc, char** argv)
{
    SetConsoleCtrlHandler(sigHandler, TRUE);

    const uint32_t numThreads = 3;
    jn::Thread threads[numThreads];
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