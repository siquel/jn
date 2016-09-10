#include <jkn/jkn.h>
#include <jkn/thread/thread.h>
#include <stdio.h> // FILE
#include <vector>
#include <string>
#include <jkn/thread/mutex.h>
/*
write a program that starts two threads. The first one reads strings
from keyboard, and the second one writes the strings to a file by speed of 10 characters per second. 
Program is terminated when user writes row exit
*/
static bool s_exit = false;
std::vector<char> buffer;

jkn::Mutex bufferMutex;

int32_t fileRoutine(void*)
{
    FILE* file = NULL;
    const char fileName[] = "03-filethread.txt";
    file = fopen(fileName, "w");

    if (file == NULL) return 1;

    uint32_t index = 0;

    while (!s_exit)
    {
        jkn::ScopedMutex lock(bufferMutex);
        uint32_t last = index + 10;
        for (uint32_t i = index; i < last && i < buffer.size(); ++i)
        {
            fwrite(&buffer[i], sizeof(char), 1, file);
            index += 1;
            ::Sleep(100);
        }
    }

    fclose(file);

    return 0;
}

int32_t inputFunc(void*)
{
    while (!s_exit)
    {
        char line[64];
        fgets(line, sizeof(line), stdin);

        if (strcmp(line, "exit\n") == 0)
        {
            s_exit = true;
            break;
        }

        size_t len = strlen(line);

        bufferMutex.lock();

        for (size_t i = 0; i < len; ++i)
        {
            buffer.push_back(line[i]);
        }

        bufferMutex.unlock();
    }

    return 0;
}

int main(int argc, char** argv)
{
    jkn::Thread osThread;
    jkn::Thread fileThread;

    buffer.reserve(1024);

    fileThread.start(fileRoutine);
    osThread.start(inputFunc);

    fileThread.join();
    osThread.join();

    return 0;
}