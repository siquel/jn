#include <vector> // vector
#include <time.h> // time
#include <jkn/macros.h>
#include <jkn/thread/thread.h>
#include <jkn/thread/mutex.h>
#include <jkn/os.h>
#include <jkn/platform.h>
#include <numeric> // accumulate
#include <stdlib.h> // rand
#include <stdio.h> // printf
/*
Implement a program whereone thread draw random numbers from interval 1. . . 10 once a second,
second thread prints the random numbers using std::cout, and main thread waits enter button. 
After the button hit software terminates. Before termination the software prints sum of all
random numbers displayed.
*/

static bool s_exit = false;

uint32_t index = 0;
std::vector<int32_t> buffer;

jkn::Mutex bufferMutex;
jkn::Mutex indexMutex;

int32_t printProc(void*)
{
    while (!s_exit)
    {
        bufferMutex.lock();
        indexMutex.lock();

        while (index < buffer.size())
        {
            printf("%d (size = %d)\n", buffer[index], uint32_t(buffer.size()));
            ++index;
        }

        indexMutex.unlock();
        bufferMutex.unlock();
    }

    return 0;
}

int32_t rngProc(void*)
{
    while (!s_exit)
    {
        int32_t random = rand() % 10 + 1;

        bufferMutex.lock();
        indexMutex.lock();

        buffer.push_back(random);

        indexMutex.unlock();
        bufferMutex.unlock();

        jkn::sleep(1000);
    }

    return 0;
}

int main(int /*argc*/, char** /*argv*/)
{
    srand(uint32_t(time(NULL)));
    jkn::Thread rngThread;
    jkn::Thread printThread;

    rngThread.start(rngProc);
    printThread.start(printProc);

    printf("Press ENTER to stop\n");

#if JKN_PLATFORM_WINDOWS
    // wait for enter
    while (!GetAsyncKeyState(VK_RETURN)) {}

#elif JKN_PLATFORM_LINUX
#endif
    
    s_exit = true;

    rngThread.join();
    printThread.join();

    printf("Sum = %d\n", std::accumulate(buffer.begin(), buffer.end(), 0));
    return 0;
}
