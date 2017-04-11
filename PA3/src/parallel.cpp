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

void generateNumbers(int numItems, vector<int>& data);

void bucketSort(int numItems, int *data);

void bubbleSort(vector<int>& data);

void swap(int& one, int& other);

void calcStatistics(vector<double> measurements, double &avg, double &stdDev);

void outputBucket(vector<int> bucket, int process);

int main(int argc, char *argv[])
{
    // Initialization
    int numTasks, rank, dest, src, tag = 1;
    int index, regionIndex, regionSize, bucketSize, bucketNum, bucketIndex,
        startNumber, dataIndex, srcProcess, lastRegSize, numItems = 0;
    double floatSize, average, stdDev;
    vector<int> data, region, oneBucket;
    vector< vector<int> > smallBuckets;
    Timer timer;
    vector<double> timings;
    MPI_Status status;

    // Initialize MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numTasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Check if number of elements specified
    if (argc >= 2)
    {
        // Get number of items to generate
        numItems = atoi(argv[1]);

        // Check if argument specified file output
        if (argc >= 3 && strcmp(argv[2], "y") == 0)
        {
            outputSorted = true;
        }

        if (rank == 0)
        {
            // Generate specified amount of numbers
            generateNumbers(numItems, data);

            // Send regions to other processes
            regionSize = lastRegSize = numItems / numTasks;

            if( fmod((float) numItems, (float) numTasks) != 0)
            {
                lastRegSize = numItems - (numTasks - 1) * regionSize;
            }


            region.resize(regionSize);
            smallBuckets.resize(numTasks);

            for(regionIndex = 1; regionIndex < numTasks; regionIndex++)
            {
                // Send size of one region to current process
                MPI_Send(&regionSize, 1, MPI_INT, regionIndex, tag, MPI_COMM_WORLD);

                // Set starting index of region
                startNumber =  ( regionIndex - 1 ) * regionSize;

                // Copy numbers from data array into region array
                for(dataIndex = startNumber, index = 0;
                 dataIndex < startNumber + regionSize; dataIndex++, index++)
                {
                    region[index] = data[dataIndex];
                }

                // Send current region to appropriate processes
                MPI_Send(&(region[0]), regionSize, MPI_INT, regionIndex, tag, MPI_COMM_WORLD);
            }

            // Barrier
            MPI_Barrier(MPI_COMM_WORLD);

            // Start timer
            timer.start();

            // Sort master's region into small buckets
            region.resize(lastRegSize);

//cout << "MASTER STARTED SORTING INTO BUCKETS" << endl;
            for(index = 0; index < lastRegSize; index++, dataIndex++)
            {
                bucketNum = (float) data[dataIndex] / (( (float) MAX_NUM + 1) / (float) numTasks);
                smallBuckets[bucketNum].push_back(data[dataIndex]);
            }

            // Send and receive buckets
            region.clear();

            for(index = 1; index < numTasks; index++)
            {
                // Send bucket size and bucket contents to current process
                bucketSize = smallBuckets[index].size();
                MPI_Send(&bucketSize, 1, MPI_INT, index, tag, MPI_COMM_WORLD);
                MPI_Send(&(smallBuckets[index][0]), bucketSize, MPI_INT, index, tag, MPI_COMM_WORLD);
                MPI_Barrier(MPI_COMM_WORLD);
cout << "Process: 0 SENT bucket TO " << index << " of size " << bucketSize << endl;

            }
            for(index = 1; index < numTasks; index++)
            {
                // Receive bucket size and bucket contents from current process
                MPI_Recv(&bucketSize, 1, MPI_INT, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, &status);
                srcProcess = status.MPI_SOURCE;
                oneBucket.resize(bucketSize);
                MPI_Recv(&(oneBucket[0]), bucketSize, MPI_INT, srcProcess, tag, MPI_COMM_WORLD, &status);

cout << "Process: 0 got bucket from " << srcProcess << " of size " << bucketSize << endl;

                // Copy contents to region (big bucket)
                for(dataIndex = 0; dataIndex < bucketSize; dataIndex++)
                {
                    region.push_back(oneBucket[dataIndex]);
                }
            }
            MPI_Barrier(MPI_COMM_WORLD);

//cout << "MASTER STARTING REGION SORT" << endl;
            // Sort final bucket
            bubbleSort(region);
//cout << "MASTER FINISHED REGION SORT" << endl;
cout << "TASK " << rank << " HAS " << region.size() << " ITEMS." << endl;
            // Barrier
            MPI_Barrier(MPI_COMM_WORLD);

            // Stop timer
            timer.stop();
            timings.push_back(timer.getElapsedTime());

            // Optionally output to file
            if(outputSorted)
            {
                smallBuckets.resize(regionSize);
                for(index = 1; index < numTasks; index++)
                {
                    MPI_Recv(&regionSize, 1, MPI_INT, index, tag, MPI_COMM_WORLD, &status);
                    smallBuckets[index - 1].resize(regionSize);
                    MPI_Recv(&(smallBuckets[index - 1][0]), regionSize, MPI_INT, index, tag, MPI_COMM_WORLD, &status);
                    MPI_Barrier(MPI_COMM_WORLD);
                }

                ofstream fout;
                fout.open("par_sorted.txt");
                for(bucketIndex = 0; bucketIndex < numTasks - 1; bucketIndex++)
                {
                    for(index = 0; index < smallBuckets[bucketIndex].size(); index++)
                    {
                        fout << smallBuckets[bucketIndex][index] << endl;
                    }
                }

                for(index = 0; index < region.size(); index++)
                {
                    fout << region[index] << endl;
                }
                fout.close();
            }

            calcStatistics(timings, average, stdDev);
        }
        else
        {
            // Receive region
            MPI_Recv(&regionSize, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
            region.resize(regionSize);
            MPI_Recv(&(region[0]), regionSize, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);

            // Barrier
            MPI_Barrier(MPI_COMM_WORLD);

            // Sort numbers into small buckets
            smallBuckets.resize(numTasks);
            for(index = 0; index < regionSize; index++)
            {
                bucketNum = (float) region[index] / (( (float) MAX_NUM + 1) / (float) numTasks);
                smallBuckets[bucketNum].push_back(region[index]);
            }

            // Send and receive buckets
            region.clear();

            for(index = 1; index < numTasks; index++)
            {
                if( rank == index)
                {
                    // Get buckets from all other processes
                    for(bucketIndex = 0; bucketIndex < numTasks - 1; bucketIndex++)
                    {
                        // Receive bucket size and bucket contents from current process
                        MPI_Recv(&bucketSize, 1, MPI_INT, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, &status);
                        srcProcess = status.MPI_SOURCE;
                        oneBucket.resize(bucketSize);
                        MPI_Recv(&(oneBucket[0]), bucketSize, MPI_INT, srcProcess, tag, MPI_COMM_WORLD, &status);

cout << "Process: " << rank << " got bucket from " << srcProcess << " of size " << bucketSize << endl;

                        // Copy contents to region (big bucket)
                        for(dataIndex = 0; dataIndex < bucketSize; dataIndex++)
                        {
                            region.push_back(oneBucket[dataIndex]);
                        }

                    }
                    MPI_Barrier(MPI_COMM_WORLD);
                }
                else
                {
                    // Send bucket size and bucket contents to current process
                    bucketSize = smallBuckets[index].size();
                    MPI_Send(&bucketSize, 1, MPI_INT, index, tag, MPI_COMM_WORLD);
                    MPI_Send(&(smallBuckets[index][0]), bucketSize, MPI_INT, index, tag, MPI_COMM_WORLD);

cout << "Process: " << rank << " SENT bucket TO " << index << " of size " << bucketSize << endl;

                    MPI_Barrier(MPI_COMM_WORLD);
                }
            }
            // Send last bucket to master
            bucketSize = smallBuckets[bucketIndex].size();
            MPI_Send(&bucketSize, 1, MPI_INT, 0, tag, MPI_COMM_WORLD);
            MPI_Send(&(smallBuckets[bucketIndex][0]), bucketSize, MPI_INT, 0, tag, MPI_COMM_WORLD);

cout << "Process: " << rank << " SENT bucket TO 0 of size " << bucketSize << endl;

            MPI_Barrier(MPI_COMM_WORLD);

//cout << "TASK " << rank << " STARTING REGION SORT" << endl;
            // Sort final bucket
            bubbleSort(region);
//cout << "TASK " << rank << " FINISHED REGION SORT" << endl;

            cout << "TASK " << rank << " HAS " << region.size() << " ITEMS." << endl;

            // Barrier
            MPI_Barrier(MPI_COMM_WORLD);

            regionSize = region.size();

            if(outputSorted)
            {
                for(index = 1; index < numTasks; index++)
                {
                    if(rank == index)
                    {
                        MPI_Send(&regionSize, 1, MPI_INT, 0, tag, MPI_COMM_WORLD);
                        MPI_Send(&(region[0]), regionSize, MPI_INT, 0, tag, MPI_COMM_WORLD);
                    }
                    MPI_Barrier(MPI_COMM_WORLD);
                }
            }
        }
    }

    // Shut down
    MPI_Finalize();

    // Exit program
    return 0;
}

void generateNumbers(int numItems, vector<int>& data)
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
        for(int index = 1; index < data.size(); index++)
        {
            // Check if current item needs to be swapped
            if(data[index - 1] > data[index])
            {
                // Swap current item
                swap(data[index - 1 ], data[index]);

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