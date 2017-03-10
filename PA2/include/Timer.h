#ifndef TIMER_H
#define TIMER_H

#include <sys/time.h>

using namespace std;

class Timer
{
  public:
    Timer();
    ~Timer();

    void start();
    void stop();
    void resume();
    double getElapsedTime();

  private:
    struct timeval startData, endData;
    long int startTime, endTime;
    long int seconds, microSeconds;
    bool validTime, isRunning;
    double totalTime;
};

#endif 

