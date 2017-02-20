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

void Timer::getElapsedTime(char *timeStr)
{
    cout << "Seconds: " << seconds << endl << "MicroSeconds: " << microSeconds << endl;
      
    totalTime =  (double) seconds + (double) microSeconds / 1000000;

    cout << "Total: " << setprecision(6) << totalTime;

    int low, high, index = 0;
    char temp;

    if (validTime)
    {
        seconds = endData.tv_sec - startData.tv_sec;
        microSeconds = endData.tv_usec - startData.tv_usec;

        if (microSeconds < 0)
        {
            microSeconds += 1000000;
            seconds -= 1;
        }

        while (microSeconds > 0)
        {
            timeStr[index] = char(microSeconds % 10 + '0');
            microSeconds /= 10;
            index++;
        }

        while (index < 6)
        {
            timeStr[index] = '0';
            index++;
        }

        timeStr[index] = RADIX_POINT;
        index++;

        if (seconds == 0)
        {
            timeStr[index] = '0';
            index++;
        }

        while (seconds > 0)
        {
            timeStr[index] = char(seconds % 10 + '0');
            seconds /= 10;
            index++;
        }

        timeStr[index] = NULL_CHAR;

        low = 0;
        high = index - 1;

        while (low < high)
        {
            temp = timeStr[low];
            timeStr[low] = timeStr[high];
            timeStr[high] = temp;

            low++;
            high--;
        }
    }

    else
    {
        strcpy(timeStr, "No Data");
    }
    
}