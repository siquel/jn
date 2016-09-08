#include <jn/jn.h>
#include <stdio.h>
#include <jn/thread/thread.h>


int32_t func(void*)
{
    printf("Hello from thread\n");
    return 55;
}

int main(int argc, char** argv)
{
    jn::Thread thread;
    thread.start(func);
    thread.join();

    printf("Thread exited with code %d\n", thread.getExitCode());
    return 0;
}