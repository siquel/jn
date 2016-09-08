#include <vector> // vector
#include <time.h> // time
#include <jkn/jkn.h>
#include <jkn/thread/thread.h>
/*
Implement a program whereone thread draw random numbers from interval 1. . . 10 once a second,
second thread prints the random numbers using std::cout, and main thread waits enter button. 
After the button hit software terminates. Before termination the software prints sum of all
random numbers displayed.
*/

static bool s_exit = false;

std::vector<int32_t> buffer;

int32_t printProc(void*)
{
    return 0;
}

int32_t rngProc(void*)
{
    while (!s_exit)
    {
        int32_t random = rand() % 10 + 1;
        ::Sleep(1000);
    }

    return 0;
}

int main(int argc, char** argv)
{
    srand(uint32_t(time(NULL)));
    jkn::Thread rngThread;
    jkn::Thread printThread;
    return 0;
}