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

const float IMG_WIDTH = 28000;
const float IMG_HEIGHT = 28000;

const int NUM_MEASUREMENTS = 1;
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
    int rowIndex, colIndex, index;
    int w = INT_WIDTH;
    int h = INT_HEIGHT;
    double average, stdDev;
    char f[20] = "imageSeq.pim";
    char *fPtr = f;
    Complex c;
    vector<double> timings;

        // Allocate 2D array for the pixels
        unsigned char** colors = new unsigned char*[INT_HEIGHT];

        for(int index = 0; index < INT_HEIGHT; index++ )
        {
            colors[index] = new unsigned char[INT_WIDTH];
        }

    // Scale image based on coordinates of rea/imaginary plane
    float scale_real = ( REAL_MAX - REAL_MIN )/ IMG_WIDTH;
    float scale_imag = ( IMAG_MAX - IMAG_MIN )/ IMG_HEIGHT;

    // Loop specified amount of times to get measurements
    for(index = 0; index < NUM_MEASUREMENTS; index++)
       {
        // Start the timer
        timer.start();

        // Loop through all rows of the image
        for( rowIndex = 0; rowIndex < IMG_HEIGHT; rowIndex++ )
        {
            // Loop through each pixel of the current row
            for( colIndex = 0; colIndex < IMG_WIDTH; colIndex++ )
            {            
                // Calculate current pixel's color
                c.real = REAL_MIN + ((float) colIndex * scale_real);
                c.imag = IMAG_MIN + ((float) rowIndex * scale_imag);

                colors[rowIndex][colIndex] = cal_pixel(c);
            }
            // end pixel loop
        }
        // end row loop

        // Stop the timer and store the time
        timer.stop();
        timings.push_back(timer.getElapsedTime());
       }
    // end outer loop

    // Calculate statistics of timings
    calcStatistics(timings, average, stdDev);

    // Write pixel colors to file
    pim_write_black_and_white(fPtr, w, h, (const unsigned char**) colors);
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
