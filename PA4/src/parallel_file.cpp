/*******************************************************************************
FILE: parallel_file.cpp

Description: Program that performs parallel matrix multiplication using Canon's
algorith. This version loads the A and B matrices from individual files passed
in as command-line parameters, then outputs the resulting C matrix to a file 
called "output.txt".

*******************************************************************************/

#include <mpi.h>
#include <cstdlib>
#include <time.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <stdio.h>
#include "Timer.h"
#include <string.h>
#include <random>
#include <cmath>

using namespace std;

const int NUM_MEASUREMENTS = 1;
const int MIN_NUM = 0;
const int MAX_NUM = 9;

/*
Purpose: Function used to ensure the chunk matrices are the correct size. 
Adjusts number of tasks incrementally down until it's a perfect square.
*/
bool fileInput(int &matrixSize, char *fileA, char *fileB, vector<vector<int>> &A,
               vector<vector<int>> &B, vector<vector<int>> &C);

void sendChunksFromMaster(int matrixSize, int offset, int numTasks, MPI_Comm comm,
                          vector<vector<int>> A, vector<vector<int>> B);

void matrixMult(vector<vector<int>> A, vector<vector<int>> B,
                vector<vector<int>> &C);

void calcStatistics(vector<double> measurements, double &avg, double &stdDev);

void outputResults(vector<vector<int>> A, vector<vector<int>> B,
                   vector<vector<int>> chunkC, int rank, int numTasks, MPI_Comm cartComm);

int main(int argc, char *argv[])
{
    // Initialization
    int numTasks, rank, dest, src, tag = 1;
    int index, rowIndex, colIndex, procIndex, offset, matrixSize = 0;
    vector<vector<int>> A, B, C, chunkA, chunkB, chunkC;
    double average, stdDev;
    Timer timer;
    vector<double> timings;
    bool outputMatrices = false;
    int periods[2] = {1, 1};
    int dimensions[2] = {0, 0};
    int coords[2] = {-1, -1};
    MPI_Status status;
    MPI_Comm cartComm;

    // Initialize MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numTasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Dims_create(numTasks, 2, dimensions);
    MPI_Cart_create(MPI_COMM_WORLD, 2, dimensions, periods, 0, &cartComm);
    MPI_Cart_coords(cartComm, rank, 2, coords);

    // Check if at least both filenames specified
    if (argc >= 3 && numTasks > 3)
    {
        // Make sure number of tasks is a perfect square
        while (fmod(sqrt((double)numTasks), 1.0) != 0)
        {
            numTasks--;

            if (rank == 0)
                cout << "Task count adjusted to " << numTasks << endl;
        }

        // Check if argument 4 specified file output
        if (argc >= 4 && strcmp(argv[3], "y") == 0)
        {
            outputMatrices = true;
        }

        // Master reads matrices from file and sends chunks out
        if (rank == 0)
        {
            // Perform file input, exit if error
            if (!fileInput(matrixSize, argv[1], argv[2], A, B, C))
            {
                return 0;
            }
            cout << "hey" << endl;

            // Set number of elements per row/col in chunk, allocate memory
            offset = matrixSize / sqrt(numTasks);

            chunkA.resize(offset);
            chunkB.resize(offset);

            // Copy own chunks of A and B
            for (index = 0; index < offset; index++)
            {
                for (colIndex = 0; colIndex < offset; colIndex++)
                {
                    chunkA[index].push_back(A[index][colIndex]);
                    chunkB[index].push_back(B[index][colIndex]);
                }
            }

cout << "master sending chunks" << endl;

            // Send each other process their portion
            sendChunksFromMaster(matrixSize, offset, numTasks, cartComm, A, B);
        }
        // Other nodes receive their chunk from master
        else if (rank < numTasks)
        {
            // Get matrix size
            MPI_Recv(&matrixSize, 1, MPI_INT, 0, tag, cartComm, &status);

            cout << "task " << rank << " got mat size of " << matrixSize << endl;

            // Set number of elements per row/col in chunk, allocate memory
            offset = matrixSize / sqrt(numTasks);

            chunkA.resize(offset);
            chunkB.resize(offset);

            // Receive matrix size and chunks of A and B
            for (index = 0; index < offset; index++)
            {
                // Resize row of A and B to be able to fit chunk row
                chunkA[index].resize(offset);
                chunkB[index].resize(offset);

                // Get current row of chunks for A and B
                MPI_Recv(&(chunkA[index][0]), offset, MPI_INT, 0, tag, cartComm, &status);
                MPI_Recv(&(chunkB[index][0]), offset, MPI_INT, 0, tag, cartComm, &status);
            }

            cout << "task " << rank << " got chunk" << endl;
        }
            cout << "task " << rank << " hit barrier" << endl;

        // Barrier
        MPI_Barrier(cartComm);

        cout << "Finished sending chunks" << endl;

        // Start the timer
        timer.start();

        // Cannon's Algorithm - Initialize Rows
        if (coords[0] != 0)
        {
            // Shift A rows i places left
            MPI_Cart_shift(cartComm, 1, -coords[0], &src, &dest);
            for (index = 0; index < offset; index++)
            {
                MPI_Sendrecv_replace(&(chunkA[index][0]), offset, MPI_INT, dest,
                                     tag, src, tag, cartComm, &status);
            }
        }

        // Barrier
        MPI_Barrier(cartComm);

        // Cannon's Algorithm - Initialize Columns
        if (coords[1] != 0)
        {
            // Shift B columns j places up
            MPI_Cart_shift(cartComm, 0, -coords[1], &src, &dest);
            for (index = 0; index < offset; index++)
            {
                MPI_Sendrecv_replace(&(chunkB[index][0]), offset, MPI_INT, dest,
                                     tag, src, tag, cartComm, &status);
            }
        }

        // Barrier
        MPI_Barrier(cartComm);

        // Cannon's Algorithm - Shift and multiply sqrt(numTasks) times
        for (int shiftIndex = 0; shiftIndex < sqrt(numTasks); shiftIndex++)
        {
            // Cannon's Algorithm - Shift A rows once left
            MPI_Cart_shift(cartComm, 1, -1, &src, &dest);
            for (index = 0; index < offset; index++)
            {
                MPI_Sendrecv_replace(&(chunkA[index][0]), offset, MPI_INT, dest,
                                     tag, src, tag, cartComm, &status);
            }

            // Cannon's Algorithm - Shift B cols once up
            MPI_Cart_shift(cartComm, 0, -1, &src, &dest);
            for (index = 0; index < offset; index++)
            {
                MPI_Sendrecv_replace(&(chunkB[index][0]), offset, MPI_INT, dest,
                                     tag, src, tag, cartComm, &status);
            }

            // Barrier
            MPI_Barrier(cartComm);

            // Multiply
            matrixMult(chunkA, chunkB, chunkC);
        }

        // Barrier
        MPI_Barrier(cartComm);

        // Stop the timer
        timer.stop();
        timings.push_back(timer.getElapsedTime());

        // Calculate statistics about timings
        if (rank == 0)
        {
            calcStatistics(timings, average, stdDev);
        }

        // Output results if flag indicates
        if (outputMatrices)
        {
            outputResults(A, B, chunkC, rank, numTasks, cartComm);
        }
    }

    // Shut down
    MPI_Finalize();

    // Exit program
    return 0;
}

bool fileInput(int &matrixSize, char *fileA, char *fileB, vector<vector<int>> &A,
               vector<vector<int>> &B, vector<vector<int>> &C)
{
    // Initialize function/variables
    int index, rowIndex, colIndex, dummy, temp;
    ifstream finA, finB;

    // Clean and open both input files
    finA.clear();
    finB.clear();
    finA.open(fileA);
    finB.open(fileB);

    // Check if both files opened good
    if (finA.good() && finB.good())
    {
        // Read matrix size
        finA >> matrixSize;
        finB >> dummy;

        // Size matrices appropriately
        A.resize(matrixSize);
        B.resize(matrixSize);
        C.resize(matrixSize);

        // Set A, B and C matrices
        for (rowIndex = 0; rowIndex < matrixSize; rowIndex++)
        {
            // Loop through each column
            for (colIndex = 0; colIndex < matrixSize; colIndex++)
            {
                // Read values for A and B
                finA >> temp;
                A[rowIndex].push_back(temp);
                finB >> temp;
                B[rowIndex].push_back(temp);

                // Initialize C value
                C[rowIndex].push_back(0);
            }
        }
        // end loop

        // Close files
        finA.close();
        finB.close();
    }
    else
    {
        cout << "FILE ERROR: Ensure input files are specified correctly" << endl;
        return false;
    }

    return true;
}

void sendChunksFromMaster(int matrixSize, int offset, int numTasks, MPI_Comm comm,
                          vector<vector<int>> A, vector<vector<int>> B)
{
    // Initialization
    int procIndex, rowIndex, colIndex, index, tag = 1;

    // Send each process the matrix size
    for (index = 0; index < numTasks; index++)
    {
        MPI_Send(&matrixSize, 1, MPI_INT, index, tag, comm);
    }

    // Send each process their chunks
    procIndex = 0;
    for (rowIndex = 0; rowIndex < sqrt(numTasks); rowIndex++)
    {
        for (colIndex = 0; colIndex < sqrt(numTasks); colIndex++, procIndex++)
        {
            // Make sure master is skipped
            if (procIndex != 0)
            {
                // Send current process their chunks of A and B
                for (index = 0; index < offset; index++)
                {
                    // Send current row portion of A
                    MPI_Send(&(A[(rowIndex * offset) + index][colIndex * offset]),
                             offset, MPI_INT, procIndex, tag, comm);

                    // Send current row portion of B
                    MPI_Send(&(B[(rowIndex * offset) + index][colIndex * offset]),
                             offset, MPI_INT, procIndex, tag, comm);
                }
            }
        }
    }
}

void matrixMult(vector<vector<int>> A, vector<vector<int>> B,
                vector<vector<int>> &C)
{
    // Initialize function/variables
    int index, rowIndex, colIndex;

    // Multiply matrices
    for (rowIndex = 0; rowIndex < C.size(); rowIndex++)
    {
        // Loop through each column
        for (colIndex = 0; colIndex < C.size(); colIndex++)
        {
            for (index = 0; index < C.size(); index++)
            {
                C[rowIndex][colIndex] += A[rowIndex][index] * B[index][colIndex];
            }
        }
        // end inner loop
    }
    // end outer loop
}

void outputResults(vector<vector<int>> A, vector<vector<int>> B,
                   vector<vector<int>> chunkC, int rank, int numTasks, MPI_Comm cartComm)
{
    // initialize function/variables
    int rowIndex, colIndex, offset = chunkC.size();
    int coords[2];

    // Get coordinates of process
    MPI_Cart_coords(cartComm, rank, 2, coords);

    // Master output A and B matrices
    if (rank == 0)
    {
        // Output matrix A
        for (rowIndex = 0; rowIndex < A.size(); rowIndex++)
        {
            for (colIndex = 0; colIndex < A.size(); colIndex++)
            {
                // Output current item
                cout << A[rowIndex][colIndex] << " ";
            }
            cout << endl;
            // end inner loop
        }
        cout << endl
             << endl
             << endl;
        // end outer loop

        // Output matrix B
        for (rowIndex = 0; rowIndex < B.size(); rowIndex++)
        {
            for (colIndex = 0; colIndex < B.size(); colIndex++)
            {
                // Output current item
                cout << B[rowIndex][colIndex] << " ";
            }
            cout << endl;
            // end inner loop
        }
        cout << endl
             << endl
             << endl;
        // end outer loop
    }
    // Everyone output their chunks of C
    for (int pIndex = 0; pIndex < numTasks; pIndex++)
    {
        // Check if turn to output
        if (pIndex == rank)
        {
            // Output current process' chunk of matrix C
            cout << "Process " << rank
                 << " at [" << coords[0]
                 << ", " << coords[1]
                 << "], results:"
                 << endl;

            for (rowIndex = 0; rowIndex < offset; rowIndex++)
            {
                for (colIndex = 0; colIndex < offset; colIndex++)
                {
                    cout << chunkC[rowIndex][colIndex] << " ";
                }
                cout << endl;
            }
            cout << endl;
        }

        // Barrier
        MPI_Barrier(cartComm);
    }
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