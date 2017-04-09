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

using namespace std;

const int NUM_MEASUREMENTS = 1;
const int MIN_NUM = 0;
const int MAX_NUM = 999999;

bool outputSorted = false;

void generateNumbers(int numItems, vector<int> &data);

void bucketSort(int numItems, int *data);

void bubbleSort(vector<int> &data);

void swap(int &one, int &other);

void calcStatistics(vector<double> measurements, double &avg, double &stdDev);

void outputBucket(vector<int> bucket, int process);

int main(int argc, char *argv[])
{
    // Initialization
    int numTasks, rank, dest, src, tag = 1;
    int index, regionIndex, regionSize, bucketSize, bucketNum,
        startNumber, dataIndex, srcProcess, lastRegSize, numItems = 0;
    double floatSize;
    vector<int> data, region, oneBucket;
    vector<vector<int>> smallBuckets;
    Timer timer;
    vector<double> timings;
    MPI_Status status;

    // Initialize MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numTasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0)
    {
        bucketSize = 100;
        oneBucket.resize(100);
        
        for(index = 0; index < bucketSize; index++)
        {
            oneBucket[index] = index;
        }

        MPI_Send(&bucketSize, 1, MPI_INT, 1, tag, MPI_COMM_WORLD);
        MPI_Send(&(oneBucket[0]), bucketSize, MPI_INT, index, tag, MPI_COMM_WORLD);
    }
    else
    {
        MPI_Recv(&bucketSize, 1, MPI_INT, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, &status);
        srcProcess = status.MPI_SOURCE;
        oneBucket.resize(bucketSize);
        MPI_Recv(&(oneBucket[0]), bucketSize, MPI_INT, srcProcess, tag, MPI_COMM_WORLD, &status);

        cout << "Process " << rank << " got bucket from " << status.MPI_SOURCE << " of size " << bucketSize << ", contents: " << endl;

        for(index = 0; index < bucketSize; index++)
        {
            cout << oneBucket[index] << endl;
        }
        

    }

    // Exit program
    return 0;
}

void generateNumbers(int numItems, vector<int> &data)
{
    // Initialize function/variables
    int index;
    random_device rd;
    default_random_engine generator(rd());
    string str = "new seed";
    seed_seq seed(str.begin(), str.end());
    generator.seed(seed);
    uniform_int_distribution<int> dist1(MIN_NUM, MAX_NUM);

    // Fill vector with random numbers
    for (index = 0; index < numItems; index++)
    {
        data.push_back(dist1(generator));
    }
}

void bubbleSort(vector<int> &data)
{
    // Initialize function/variables
    bool swapped = true;
    int index;

    // Check if swap occured
    while (swapped)
    {
        // Unset swapped flag
        swapped = false;

        // Loop through all items
        for (int index = 0; index < data.size() - 1; index++)
        {
            // Check if current item needs to be swapped
            if (data[index] > data[index + 1])
            {
                // Swap current item
                swap(data[index], data[index + 1]);

                // set swapped flag
                swapped = true;
            }
        }
    }
}

void swap(int &one, int &other)
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

void outputBucket(vector<int> bucket, int process)
{
    // Initialization
    int index;

    // Loop through current bucket
    for (vector<int>::iterator it = bucket.begin();
         it != bucket.end(); ++it)
    {
        // Output current item
        cout << *it << endl;
    }
    // end loop
}