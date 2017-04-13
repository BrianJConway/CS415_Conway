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
const int MAX_NUM = 9;

bool output = false;

void generateNumbers(int numItems, vector<vector<int>> &A,
                     vector<vector<int>> &B, vector<vector<int>> &C);

void matrixMult(int numItems, vector<vector<int>> A,
                vector<vector<int>> B, vector<vector<int>> &C);

void calcStatistics(vector<double> measurements, double &avg, double &stdDev);

void outputMatrices(vector<vector<int>> A,
                    vector<vector<int>> B, vector<vector<int>> C);

int main(int argc, char *argv[])
{
    // Initialization
    int index, numItems = 0;
    vector<vector<int>> A, B, C;

    // Check if number of elements specified
    if (argc >= 2)
    {
        // Get number of items to generate
        numItems = atoi(argv[1]);

        // Check if argument specified file output
        if (argc >= 3 && strcmp(argv[2], "y") == 0)
        {
            output = true;
        }

        // Generate specified amount of numbers
        generateNumbers(numItems, A, B, C);

        // Bucket sort
        matrixMult(numItems, A, B, C);
    }

    // Exit program
    return 0;
}

void generateNumbers(int numItems, vector<vector<int>> &A,
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
    A.resize(numItems);
    B.resize(numItems);
    C.resize(numItems);

    // Set A and C matrices
    for (rowIndex = 0; rowIndex < numItems; rowIndex++)
    {
        // Loop through each column
        for (colIndex = 0; colIndex < numItems; colIndex++)
        {
            // Set random value for A, set zero for C
            A[rowIndex].push_back(dist1(generator));
            C[rowIndex].push_back(0);
        }
    }
    // end loop

    // Set B matrix
    for (rowIndex = 0; rowIndex < numItems; rowIndex++)
    {
        // Loop through each column
        for (colIndex = 0; colIndex < numItems; colIndex++)
        {
            // Set random value for A, set zero for C
            B[rowIndex].push_back(dist1(generator));
        }
    }
    // end loop
}

void matrixMult(int numItems, vector<vector<int>> A,
                vector<vector<int>> B, vector<vector<int>> &C)
{
    // Initialize function/variables
    int index, rowIndex, colIndex, mIndex;
    double average, stdDev;
    Timer timer;
    vector<double> timings;

    // Loop specified amount of times to get measurements
    for (mIndex = 0; mIndex < NUM_MEASUREMENTS; mIndex++)
    {

        // Start the timer
        timer.start();

        // Multiply matrices
        for (rowIndex = 0; rowIndex < numItems; rowIndex++)
        {
            // Loop through each column
            for (colIndex = 0; colIndex < numItems; colIndex++)
            {
                for (index = 0; index < numItems; index++)
                {
                    C[rowIndex][colIndex] += A[rowIndex][index] * B[index][colIndex];
                }
            }
            // end innest loop
        }
        // end inner loop

        // Stop the timer, store time
        timer.stop();
        timings.push_back(timer.getElapsedTime());
    }
    // end outer loop

    if (output)
    {
        outputMatrices(A, B, C);
    }

    // Calculate statistics of timings
    calcStatistics(timings, average, stdDev);
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

void outputMatrices(vector<vector<int>> A,
                    vector<vector<int>> B, vector<vector<int>> C)
{
    // Initialization
    int rowIndex, colIndex;
    ofstream fout;

    // Open output file
    fout.open("output.txt");
    int count = 0;

    // Output matrix A
    for (rowIndex = 0; rowIndex < A.size(); rowIndex++)
    {
        for (colIndex = 0; colIndex < A.size(); colIndex++)
        {
            // Output current item
            fout << A[rowIndex][colIndex] << " ";
        }
        fout << endl;
        // end inner loop
    }
    fout << endl << endl << endl;
    // end outer loop

    // Output matrix B
    for (rowIndex = 0; rowIndex < B.size(); rowIndex++)
    {
        for (colIndex = 0; colIndex < B.size(); colIndex++)
        {
            // Output current item
            fout << B[rowIndex][colIndex] << " ";
        }
        fout << endl;
        // end inner loop
    }
    fout << endl << endl << endl;
    // end outer loop

    // Output matrix C
    for (rowIndex = 0; rowIndex < C.size(); rowIndex++)
    {
        for (colIndex = 0; colIndex < C.size(); colIndex++)
        {
            // Output current item
            fout << C[rowIndex][colIndex] << " ";
        }
        fout << endl;
        // end inner loop
    }
    fout << endl << endl << endl;
    // end outer loop

    // Close file
    fout.close();
}