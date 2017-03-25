#include <mpi.h>
#include <cstdlib>
#include <time.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

#include "Timer.h"
#include "PIMFuncs.h"

using namespace std;

const int NUM_MEASUREMENTS = 10;

const float IMG_WIDTH = 10000;
const float IMG_HEIGHT = 10000;

const int INT_WIDTH = IMG_WIDTH;
const int INT_HEIGHT = IMG_HEIGHT;

const float REAL_MIN = -2.0;
const float REAL_MAX = 2.0;

const float IMAG_MIN = -2.0;
const float IMAG_MAX = 2.0;

struct Complex
{
    float real;
    float imag;
};

void calcStatistics(vector<double> measurements, double& avg, double& stdDev);

int cal_pixel(Complex c);

int main(int argc, char *argv[])
{
    // Initialization
    Timer timer;
    int numTasks, rank, dest, src, tag = 1;
    int inmsg, outmsg = 1;
    int rowIndex, pixelIndex, index, procNum, rowsToSend, rowNum,
        currentRow, startingRow;
    int splitIndex, timesToSplit = 0;
    int w = INT_WIDTH;
    int h = INT_HEIGHT;
    float height = IMG_HEIGHT;
    MPI_Status status;
    double average, stdDev;
    char f[20] = "imageStat.pim";
    char *fPtr = f;
    Complex c;
    vector<double> timings;

        // Allocate 2D array for the pixels
        unsigned char** colors = new unsigned char*[INT_HEIGHT];

        for(int index = 0; index < INT_HEIGHT; index++ )
        {
            colors[index] = new unsigned char[INT_WIDTH];
        }

        // Initialize MPI
        MPI_Init(&argc, &argv);
        MPI_Comm_size(MPI_COMM_WORLD, &numTasks);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);

        // Check for proper number of tasks/proper number of rows
        if( numTasks < 2 )
        {
            MPI_Abort(MPI_COMM_WORLD, 1);
            return 0;
        }

        // Possibly adjust number of tasks used based on height and number of tasks allocated
        while( fmod(height, (float)(numTasks - 1)) != 0 )
        {
            numTasks--;

            if(rank == 0)
            cout << "NUM TASKS ADJUSTED TO " << numTasks << endl;
        }

        // Determine how many rows to send, ensuring no more than 2,000,000
        // elements in the buffer
        rowsToSend = IMG_HEIGHT / (numTasks - 1);

        while( INT_WIDTH * rowsToSend > 2000000 )
        {
            // Half the size of the buffer
            rowsToSend /= 2;
            timesToSplit++;
        }

        // Calculate number of rows to send to each processor
        unsigned char setOfRows[INT_WIDTH * rowsToSend];

        // Scale image based on coordinates of rea/imaginary plane
        float scale_real = ( REAL_MAX - REAL_MIN )/ IMG_WIDTH;
        float scale_imag = ( IMAG_MAX - IMAG_MIN )/ IMG_HEIGHT;


    // Loop specified amount of times to get measurements
    for(index = 0; index < NUM_MEASUREMENTS; index++)
    {
        // Check if task 0
        if (rank == 0)
        {
                // Start the timer
                timer.start();

                // Loop through all rows of the image
                for( rowIndex = 0, procNum = 1; rowIndex < IMG_HEIGHT; rowIndex += rowsToSend, procNum++ )
                {
                    // Send current row to corresponding process
                    MPI_Send(&rowIndex, 1, MPI_INT, procNum, tag, MPI_COMM_WORLD);
                }
                // end row loop

                // Collect results
                for(rowIndex = 0; rowIndex < IMG_HEIGHT; rowIndex += rowsToSend)
                {
                    // Get the current set of rows from a task
                    for(splitIndex = 0; splitIndex <= timesToSplit; splitIndex++ )
                    {
                        // Receive computed set of rows from any process
                        MPI_Recv(&startingRow, 1, MPI_INT, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, &status);

                        // Receive computed set of rows from any process
                        MPI_Recv(setOfRows, INT_WIDTH * rowsToSend, MPI_UNSIGNED_CHAR, status.MPI_SOURCE, tag, MPI_COMM_WORLD, &status);

                        // Copy rows to 2D array of colors
                        for(currentRow = startingRow, rowNum = 0; currentRow < startingRow + rowsToSend; currentRow++, rowNum++ )
                        {                        
                            for(pixelIndex = 0; pixelIndex < IMG_WIDTH; pixelIndex++)
                            {  
                                colors[currentRow][pixelIndex] = setOfRows[rowNum * INT_WIDTH + pixelIndex];
                            }   
                        }
                    }
     
                }

                // Stop the timer and store the time
                timer.stop();
                timings.push_back(timer.getElapsedTime());
        }
        // Otherwise, assume not task 1
        else if (rank < numTasks)
        {
            // Receive initial row number
            MPI_Recv(&startingRow, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);

            for (splitIndex = 0; splitIndex <= timesToSplit; splitIndex++)
            {
                // Loop through rows to calculate
                for (rowIndex = startingRow, rowNum = 0; rowIndex < startingRow + rowsToSend; rowIndex++, rowNum++)
                {
                    // Loop through each pixel of the current row
                    for (pixelIndex = 0; pixelIndex < IMG_WIDTH; pixelIndex++)
                    {
                        // Calculate current pixel's color
                        c.real = REAL_MIN + ((float)pixelIndex * scale_real);
                        c.imag = IMAG_MIN + ((float)rowIndex * scale_imag);

                        setOfRows[rowNum * INT_WIDTH + pixelIndex] = cal_pixel(c);
                    }
                    //end pixel loop
                }
                // end row loop

                // Send finished rows back
                currentRow = startingRow + (splitIndex * rowsToSend);
                MPI_Send(&currentRow, 1, MPI_INT, 0, tag, MPI_COMM_WORLD);
                MPI_Send(setOfRows, INT_WIDTH * rowsToSend, MPI_UNSIGNED_CHAR, 0, tag, MPI_COMM_WORLD);
            }
            // end split loop
        }
    }
    // end outer loop

    if( rank == 0 )
    {
        // Calculate statistics of timings
        calcStatistics(timings, average, stdDev);

        // Write pixel colors to file
        pim_write_black_and_white(fPtr, w, h, (const unsigned char**) colors);
    }
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
         << "Image Size: " << IMG_WIDTH << " x " << IMG_HEIGHT << endl 
         << "Average Time: " << avg << "s" << endl
         << "Standard Deviation: " << stdDev << "s" << endl;
}

int cal_pixel(Complex c)
{
    int count, max;
    Complex z;
    float temp, lengthsq;
    max = 256;
    z.real = 0;
    z.imag = 0;
    count = 0; /* number of iterations */
    do
    {
        temp = z.real * z.real - z.imag * z.imag + c.real;
        z.imag = 2 * z.real * z.imag + c.imag;
        z.real = temp;
        lengthsq = z.real * z.real + z.imag * z.imag;
        count++;
    } while ((lengthsq < 4.0) && (count < max));

    return count;
}
