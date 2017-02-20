#ifndef TIMER_H
#define TIMER_H

#include <sys/time.h>
#include <cstring>

using namespace std;

const char NULL_CHAR = '\0';
const char RADIX_POINT = '.';

class Timer
{
  public:
    Timer();
    ~Timer();

    void start();
    void stop();
    void resume();
    void getElapsedTime(char *timeStr);

  private:
    struct timeval startData, endData;
    long int startTime, endTime;
    long int seconds, microSeconds;
    bool validTime, isRunning;
    double totalTime;
};

#endif 

