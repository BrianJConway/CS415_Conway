#include <mpi.h>
#include <cstdlib>
#include <time.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <stdio.h>
#include "Timer.h"
#include <random>
#include <cmath>

using namespace std;

const int NUM_MEASUREMENTS = 1;
const int MIN_NUM = 0;
const int MAX_NUM = 999999;

bool outputSorted = false;

void generateNumbers(int matrixSize, vector<vector<int>> &A,
                     vector<vector<int>> &B, vector<vector<int>> &C);

void matrixMult(int matrixSize, vector<vector<int>> A,
                vector<vector<int>> B, vector<vector<int>> &C);

void calcStatistics(vector<double> measurements, double &avg, double &stdDev);

int main(int argc, char *argv[])
{
    // Initialization
    int numTasks, rank, dest, src, tag = 1;
    int index, matrixSize = 0;
    vector<vector<int>> A, B, C;
    double average, stdDev;
    Timer timer;
    vector<double> timings;
    int periods[2] = {1,1};
    int dimensions[2] = {0,0};
    int coords[2] = {-1,-1};
    MPI_Status status;
    MPI_Comm cartComm;

    // Initialize MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numTasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Dims_create(numTasks, 2, dimensions);
    MPI_Cart_create(MPI_COMM_WORLD, 2, dimensions, periods, 0, &cartComm); 
    MPI_Cart_coords(cartComm, rank, 2, coords);

    // Check if number of elements specified
    if (argc >= 2)
    {
        for(int index = 0; index < numTasks; index++)
        {
            if(rank == index)
            {
                cout << "Rank: " << rank 
                     << " at coords " << coords[0] << " " << coords[1] 
                     << endl;
            }
            MPI_Barrier(cartComm);
        }
    

/*
        // Get number of items to generate
        matrixSize = atoi(argv[1]);

        // Check if argument specified file output
        if (argc >= 3 && strcmp(argv[2], "y") == 0)
        {
            outputSorted = true;
        }

        // Check if master
        if (rank == 0)
        {
            // Generate specified amount of numbers
            generateNumbers(matrixSize, A, B, C);

            // Barrier
            MPI_Barrier(MPI_COMM_WORLD);
        }
        else
        {
            // Receive region
            MPI_Recv(&regionSize, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
            region.resize(regionSize);
            MPI_Recv(&(region[0]), regionSize, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);

            // Barrier
            MPI_Barrier(MPI_COMM_WORLD);

            // Barrier
            MPI_Barrier(MPI_COMM_WORLD);

            if (outputSorted)
            {

            }
        }
*/
    }

    // Shut down
    MPI_Finalize();

    // Exit program
    return 0;
}

void generateNumbers(int matrixSize, vector<vector<int>> &A,
                     vector<vector<int>> &B, vector<vector<int>> &C)
{
    // Initialize function/variables
    int index, rowIndex, colIndex;
    random_device rd;
    default_random_engine generator(rd());
    string str = "new seed";
    seed_seq seed(str.begin(), str.end());
    generator.seed(seed);
    uniform_int_distribution<int> dist1(MIN_NUM, MAX_NUM);

    // Size matrices appropriately
    A.resize(matrixSize);
    B.resize(matrixSize);
    C.resize(matrixSize);

    // Set A and C matrices
    for (rowIndex = 0; rowIndex < matrixSize; rowIndex++)
    {
        // Loop through each column
        for (colIndex = 0; colIndex < matrixSize; colIndex++)
        {
            // Set random value for A, set zero for C
            A[rowIndex].push_back(dist1(generator));
            C[rowIndex].push_back(0);
        }
    }
    // end loop

    // Set B matrix
    for (rowIndex = 0; rowIndex < matrixSize; rowIndex++)
    {
        // Loop through each column
        for (colIndex = 0; colIndex < matrixSize; colIndex++)
        {
            // Set random value for A, set zero for C
            B[rowIndex].push_back(dist1(generator));
        }
    }
    // end loop
}

void matrixMult(int matrixSize, vector<vector<int>> A,
                vector<vector<int>> B, vector<vector<int>> &C)
{
    // Initialize function/variables
    int index, rowIndex, colIndex, mIndex;
    double average, stdDev;

    // Loop specified amount of times to get measurements
    for (mIndex = 0; mIndex < NUM_MEASUREMENTS; mIndex++)
    {
        // Multiply matrices
        for (rowIndex = 0; rowIndex < matrixSize; rowIndex++)
        {
            // Loop through each column
            for (colIndex = 0; colIndex < matrixSize; colIndex++)
            {
                for (index = 0; index < matrixSize; index++)
                {
                    C[rowIndex][colIndex] += A[rowIndex][index] * B[index][colIndex];
                }
            }
            // end innest loop
        }
        // end inner loop
    }
    // end outer loop
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