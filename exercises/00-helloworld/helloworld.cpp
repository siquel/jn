#include <jkn/jkn.h>
#include <stdio.h>
#include <jkn/thread/thread.h>
#include <jkn/platform.h>
/*
write a program which starts a thread that prints hello world once a second
*/

static bool s_exit = false;

int32_t func(void*)
{
    while (!s_exit)
    {
        printf("Hello World!\n");
//        ::Sleep(1000);
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

int main(int /*argc*/, char** /*argv*/)
{
#if JKN_PLATFORM_WINDOWS
    SetConsoleCtrlHandler(sigHandler, TRUE);
#endif

    jkn::Thread thread;
    thread.start(func);
    thread.join();
    return 0;
}
