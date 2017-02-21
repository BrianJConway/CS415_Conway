#include "Timer.h"
#include <iostream>
#include <iomanip>

using namespace std;

Timer::Timer()
{
    isRunning = false;
    totalTime = 0.0;
}

Timer::~Timer()
{

}


void Timer::start()
{
    gettimeofday(&startData, NULL);
    isRunning = true;
    validTime = false;
}

void Timer::stop()
{
    if (isRunning)
    {
        gettimeofday(&endData, NULL);
        isRunning = false;
        validTime = true;
    }

    else
    {
        validTime = false;
    }
}

void Timer::resume()
{
    isRunning = true;
}

double Timer::getElapsedTime()
{
    if (validTime)
    {
        seconds = endData.tv_sec - startData.tv_sec;
        microSeconds = endData.tv_usec - startData.tv_usec;
        return (double)seconds + (double)microSeconds / 1000000;
    }
}