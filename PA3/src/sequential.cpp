#include <mpi.h>
#include <cstdlib>
#include <time.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

#include "Timer.h"

using namespace std;

const int NUM_MEASUREMENTS = 1;

void calcStatistics(vector<double> measurements, double& avg, double& stdDev);

int main(int argc, char *argv[])
{
    // Initialization
    int index;
    double average, stdDev;
    Timer timer;
    vector<double> timings;

    // Loop specified amount of times to get measurements
    for(index = 0; index < NUM_MEASUREMENTS; index++)
       {
        // Start the timer
        timer.start();


        // Stop the timer and store the time
        timer.stop();
        timings.push_back(timer.getElapsedTime());
       }
    // end outer loop

    // Calculate statistics of timings
    calcStatistics(timings, average, stdDev);
}

void calcStatistics(vector<double> measurements, double& avg, double& stdDev)
{
    // Initialization
    double sum = 0.0;
    vector<double> squaredDistances;

    // Calculate average
    for( vector<double>::iterator it = measurements.begin(); it != measurements.end(); it++ )
    {
        sum += *it;
    }

    avg = sum / NUM_MEASUREMENTS;

    // Calculate standard deviation
    sum = 0.0;
    for( vector<double>::iterator it = measurements.begin(); it != measurements.end(); it++ )
    {
        sum += pow(*it - avg, 2.0);
    }

    stdDev = sqrt(sum/NUM_MEASUREMENTS);

    // Output results
    cout << "Measurements: " << NUM_MEASUREMENTS << endl
         << "Average Time: " << avg << "s" << endl
         << "Standard Deviation: " << stdDev << "s" << endl;
}