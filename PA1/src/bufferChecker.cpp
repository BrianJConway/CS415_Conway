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

const double NUM_MESSAGES = 10;
const int MAX_INTEGERS = 10000;

void outputToFile(vector<double> measurements);

int main(int argc, char *argv[])
{
    // Initialization
    int numTasks, rank, dest, src, tag = 1;
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

        for (outerIndex = 1; outerIndex < MAX_INTEGERS + 1; outerIndex++)
        {
            // Reset timer
            started = false;
            
            // Initialize array variables
            int numbers[outerIndex];
            for (index = 0; index < outerIndex; index++)
            {
                numbers[index] = 1;
            }

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

                // Send and receive the integer(s)
                MPI_Send(numbers, outerIndex, MPI_INT, dest, tag, MPI_COMM_WORLD);
                MPI_Recv(numbers, outerIndex, MPI_INT, src, tag, MPI_COMM_WORLD, &status);

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

        for (outerIndex = 1; outerIndex < MAX_INTEGERS + 1; outerIndex++)
        {
            int numbers[outerIndex];

            for (index = 0; index < NUM_MESSAGES; index++)
            {
                // Receive the integer and send it back
                MPI_Recv(numbers, outerIndex, MPI_INT, src, tag, MPI_COMM_WORLD, &status);
                MPI_Send(numbers, outerIndex, MPI_INT, dest, tag, MPI_COMM_WORLD);
            }
        }
    }

    // Check if rank 1 and output
    if (rank == 0)
    {
        outputToFile(measurements);
    }

    // Shut down
    MPI_Finalize();
}

void outputToFile(vector<double> measurements)
{
    // Initialization
    ofstream fout;

    // Open output file 
    fout.open("measurements.csv");

    // Output measurements to .csv file 
    for( vector<double>::iterator it = measurements.begin(); it != measurements.end(); it++ )
    {
        fout << *it << ",";
    }

    // Close output file 
    fout.close();
}
