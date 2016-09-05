#include <jn/jn.h>
#include <stdio.h>
#include <jn/thread/thread.h>


int32_t func(void*)
{
    printf("Hello from thread\n");
    return 0;
}

int main(int argc, char** argv)
{
    jn::Thread thread;
    thread.start(func);
    
    return 0;
}