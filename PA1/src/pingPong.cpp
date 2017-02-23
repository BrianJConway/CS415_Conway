#include <mpi.h>
#include <cstdlib>
#include <time.h>
#include <iostream>
#include "Timer.h"
#include <iomanip>
#include <algorithm>
#include <fstream>
#include <vector>
#include <cmath>

using namespace std;

const double NUM_MESSAGES = 500;
const double NUM_ITERATIONS = 1000;

void calcStatistics(vector<double> measurements, double& avg, double& stdDev);
void outputToFile(vector<double> measurements, double avg, double stdDev);

int main(int argc, char *argv[])
{
    // Initialization
    int numTasks, rank, dest, src, tag = 1;
    int inmsg, outmsg = 1;
    double average, standardDeviation;
    MPI_Status status;
    int index, outerIndex;
    Timer timer;
    bool started = false;
    vector<double> measurements;

        // Initialize MPI
        MPI_Init(&argc, &argv);
        MPI_Comm_size(MPI_COMM_WORLD, &numTasks);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Check if not enough tasks specified
    if (numTasks < 2)
    {
        cout << "Must specify at least two tasks. Terminating..." << endl;
        MPI_Abort(MPI_COMM_WORLD, 1);
        return 0;
    }

    // Check if task 1
    if (rank == 0)
    {
        // Check if more than 2 tasks specified
        if (numTasks > 2)
        {
            cout << "Ignoring extra tasks..." << endl;
        }

        dest = 1;
        src = 1;
        
        for (outerIndex = 0; outerIndex < NUM_ITERATIONS; outerIndex++)
        {
            // Reset timer 
            started = false;
            
            for (index = 0; index < NUM_MESSAGES; index++)
            {
                // Check if timer not started
                if (!started)
                {
                    // Set flag and start timer
                    started = true;
                    timer.start();
                }
                // Otherwise, assume timer already started
                else
                {
                    // Resume the timer
                    timer.resume();
                }

                // Send and receive the single integer
                MPI_Send(&outmsg, 1, MPI_INT, dest, tag, MPI_COMM_WORLD);
                MPI_Recv(&inmsg, 1, MPI_INT, src, tag, MPI_COMM_WORLD, &status);

                // Stop the timer
                timer.stop();
            }            

            // Store time taken for single ping pong
            measurements.push_back(timer.getElapsedTime() / NUM_MESSAGES);
        }
    }
    // Otherwise, check if rank 2
    else if (rank == 1)
    {
        dest = 0;
        src = 0;

        for (outerIndex = 0; outerIndex < NUM_ITERATIONS; outerIndex++)
        {
            for (index = 0; index < NUM_MESSAGES; index++)
            {
                // Receive the integer and send it back
                MPI_Recv(&inmsg, 1, MPI_INT, src, tag, MPI_COMM_WORLD, &status);
                MPI_Send(&outmsg, 1, MPI_INT, dest, tag, MPI_COMM_WORLD);
            }
        }
    }

    // Check if rank 1 for calculations and output
    if (rank == 0)
    {
        calcStatistics(measurements, average, standardDeviation);
        outputToFile(measurements, average, standardDeviation);
    }

    // Shut down
    MPI_Finalize();
}

void calcStatistics(vector<double> measurements, double& avg, double& stdDev)
{
    // Initialization
    double average, stdDev, sum = 0.0;
    vector<double> squaredDistances;

    // Calculate average
    for( vector<double>::iterator it = measurements.begin(); it != measurements.end(); it++ )
    {
        sum += *it;
    }

    average = sum / NUM_ITERATIONS;

    // Calculate standard deviation
    sum = 0.0;
    for( vector<double>::iterator it = measurements.begin(); it != measurements.end(); it++ )
    {
        sum += pow(*it - average, 2.0);
    }

    stdDev = sqrt(sum/NUM_ITERATIONS);

    // Output results
    cout << "For " << NUM_ITERATIONS << " iterations of " << NUM_MESSAGES
         << " messages each, the average for one ping pong was "
         << average << " seconds with a standard deviation of " 
         << stdDev << " seconds." << endl;
}

void outputToFile(vector<double> measurements, double avg, double stdDev)
{
    // Initialization
    ofstream fout;

    // Open output file 
    fout.open("measurements.csv");

    // Output statistics 
    cout << average << endl
         << stdDev << endl;

    // Output measurements to .csv file 
    for( vector<double>::iterator it = measurements.begin(); it != measurements.end(); it++ )
    {
        fout << *it << ",";
    }

    // Close output file 
    fout.close();
}