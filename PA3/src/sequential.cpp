#include <mpi.h>
#include <cstdlib>
#include <time.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>

#include "Timer.h"

using namespace std;

const int NUM_MEASUREMENTS = 1;
const int MAX_NUM = 1000000;
const int NUM_BUCKETS = 16;

void readFromFile(char* fileName, int& numItems, int*& data);

void bucketSort(int numItems, int* data);

void calcStatistics(vector<double> measurements, double &avg, double &stdDev);

int main(int argc, char *argv[])
{
    // Initialization
    int index, numItems = 0;
    double average, stdDev;
    Timer timer;
    vector<double> timings;
    int* data;

    // Check if filename specified
    if (argc >= 2)
    {
        // Read data from file
        readFromFile(argv[1], numItems, data);

        // Loop specified amount of times to get measurements
        for (index = 0; index < NUM_MEASUREMENTS; index++)
        {
            // Start the timer
            timer.start();

            // Bucket sort
            buckeSort(numItems, data);

            // Stop the timer and store the time
            timer.stop();
            timings.push_back(timer.getElapsedTime());
        }
        // end outer loop

        // Calculate statistics of timings
        calcStatistics(timings, average, stdDev);

        // Deallocate array of items
        if( data != NULL)
        {
            delete[] data;
        }
    }

    // Exit program
    return 0;
}

void readFromFile(char* filename, int& numItems, int*& data)
{
    // Initialize function/variables
    int index;
    ifstream fin;

    // Clear and open input file
    fin.clear();
    fin.open(filename);

    // Get number of items, allocate array
    fin >> numItems;
    data = new int[numItems];

    // Loop through all items
    for(index = 0; index < numItems; index++)
    {
        // Read current item into array
        fin >> data[index];
    }
    // end loop
}

void bucketSort(int numItems, int* data)
{
    // Initialize function/variables
    int index, bucketNum;
    vector<vector<int> buckets;

    // Loop through all numbers
    for(index = 0; index < numItems; index++)
    {
        // Place current number into proper bucket
        bucketNum = data[index] / (MAX_NUM / NUM_BUCKETS);
        buckets[bucketNum].push_back(data[index]);
    }
    // end loop

    // Loop through all buckets
    for(index = 0; index < NUM_BUCKETS; index++)
    {
        // Sort the current bucket
        sort(buckets[index].begin(), buckets[index].end());
    }
    // end loop
}

void calcStatistics(vector<double> measurements, double &avg, double &stdDev)
{
    // Initialization
    double sum = 0.0;
    vector<double> squaredDistances;

    // Calculate average
    for (vector<double>::iterator it = measurements.begin(); it != measurements.end(); it++)
    {
        sum += *it;
    }

    avg = sum / NUM_MEASUREMENTS;

    // Calculate standard deviation
    sum = 0.0;
    for (vector<double>::iterator it = measurements.begin(); it != measurements.end(); it++)
    {
        sum += pow(*it - avg, 2.0);
    }

    stdDev = sqrt(sum / NUM_MEASUREMENTS);

    // Output results
    cout << "Measurements: " << NUM_MEASUREMENTS << endl
         << "Average Time: " << avg << "s" << endl
         << "Standard Deviation: " << stdDev << "s" << endl;
}