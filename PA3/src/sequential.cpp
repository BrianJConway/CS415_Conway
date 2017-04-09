#include <mpi.h>
#include <cstdlib>
#include <time.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <stdio.h>
#include <string.h>
#include "Timer.h"
#include <random>

using namespace std;

const int NUM_MEASUREMENTS = 1;
const int MIN_NUM = 0;
const int MAX_NUM = 999999;
const int NUM_BUCKETS = 16;

bool outputSorted = false;


void generateNumbers(int numItems, int*& data);

void bucketSort(int numItems, int* data);

void bubbleSort(vector<int>& data);

void swap(int& one, int& other);

void calcStatistics(vector<double> measurements, double &avg, double &stdDev);

void outputBuckets(vector< vector<int> > buckets);


int main(int argc, char *argv[])
{
    // Initialization
    int index, numItems = 0;
    int* data = NULL;

    // Check if number of elements specified
    if(argc >= 2)
    {
        // Get number of items to generate
        numItems = atoi(argv[1]);
        
        // Check if argument specified file output
        if (argc >= 3 && strcmp(argv[2], "y") == 0)
        {
            outputSorted = true;
        }

        // Generate specified amount of numbers
        generateNumbers(numItems, data);

        // Bucket sort
        bucketSort(numItems, data);

        // Deallocate array of items
        if (data != NULL)
        {
            delete[] data;
        }
    }

    // Exit program
    return 0;
}

void generateNumbers(int numItems, int*& data)
{
    // Initialize function/variables
    int index;
    random_device rd;
    default_random_engine generator( rd() );
    string str = "new seed";
    seed_seq seed(str.begin(),str.end());
    generator.seed(seed);
    uniform_int_distribution<int> dist1( MIN_NUM, MAX_NUM );

    // Allocate array
    data = new int[numItems];
    
    // Fill array with random numbers
    for(index = 0; index < numItems; index++)
    {
        data[index] = dist1(generator);
    }
}
void bucketSort(int numItems, int* data)
{
    // Initialize function/variables
    int index, bucketNum;
    double average, stdDev;
    Timer timer;
    vector<double> timings;
    vector< vector<int> > buckets(NUM_BUCKETS);

    // Loop specified amount of times to get measurements
    for (index = 0; index < NUM_MEASUREMENTS; index++)
    {
        // Loop through all numbers
        for(index = 0; index < numItems; index++)
        {
            // Place current number into proper bucket
            bucketNum = data[index] / ((MAX_NUM + 1) / NUM_BUCKETS);
            buckets[bucketNum].push_back(data[index]);
        }
        // end loop

        // Start the timer
        timer.start();

        // Loop through all buckets
        for(index = 0; index < NUM_BUCKETS; index++)
        {
            // Sort the current bucket
            bubbleSort(buckets[index]);
        }
        // end loop

        // Stop the timer and store the time
        timer.stop();
        timings.push_back(timer.getElapsedTime());
    }
    // end outer loop

    if(outputSorted)
    {
        outputBuckets(buckets);
    }

/*
    vector<int> bucket;
    for(index = 0; index < numItems; index++)
    {
        bucket.push_back(data[index]);
    }

    timer.start();
    bubbleSort(bucket);
    timer.stop();
    timings.push_back(timer.getElapsedTime());
*/

    // Calculate statistics of timings
    calcStatistics(timings, average, stdDev);    
}

void bubbleSort(vector<int>& data)
{
    // Initialize function/variables
    bool swapped = true;
    int index;
    
    // Check if swap occured
    while(swapped)
    {
        // Unset swapped flag
        swapped = false;

        // Loop through all items
        for(int index = 0; index < data.size() - 1; index++)
        {
            // Check if current item needs to be swapped
            if(data[index] > data[index+1])
            {
                // Swap current item
                swap(data[index], data[index + 1]);

                // set swapped flag
                swapped = true;
            }
        }
    }
}

void swap(int& one, int& other)
{
    int temp = one;
    one = other;
    other = temp;
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

void outputBuckets(vector< vector<int> > buckets)
{
    // Initialization
    int index;
    ofstream fout;
    
    // Open output file
    fout.open("sorted.txt");

    // Loop through all buckets 
    for(index = 0; index < NUM_BUCKETS; index++)
    {
        // Loop through current bucket
        for(vector<int>::iterator it = buckets[index].begin();
         it != buckets[index].end(); ++it)
         {
             // Output current item
             fout << *it << endl;
         }
         // end inner loop
    }
    // end outer loop

    // Close file
    fout.close();
}