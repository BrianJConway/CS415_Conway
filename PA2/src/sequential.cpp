#include <mpi.h>
#include <cstdlib>
#include <time.h>
#include <iostream>
#include <fstream>

#include "Timer.h"
#include "PIMFuncs.h"

using namespace std;

const float IMG_WIDTH = 640;
const float IMG_HEIGHT = 480;

const int INT_WIDTH = IMG_WIDTH;
const int INT_HEIGHT = IMG_HEIGHT;

const float REAL_MIN = -2;
const float REAL_MAX = 2;

const float IMAG_MIN = -2;
const float IMAG_MAX = 2;

struct Complex
{
    float real;
    float imag;
};

int cal_pixel(Complex c);

int main(int argc, char *argv[])
{
    // Initialization
    Timer timer;
    int rowIndex, colIndex;
    unsigned char** colors = new unsigned char*[INT_HEIGHT];

    for(int index = 0; index < INT_HEIGHT; index++ )
    {
        colors[index] = new unsigned char[INT_WIDTH];
    }

    int w = INT_WIDTH;
    int h = INT_HEIGHT;
    char f[10] = "a";
    char *fPtr = f;

    Complex c;

    float scale_real = ( REAL_MAX - REAL_MIN )/ IMG_WIDTH;
    float scale_imag = ( IMAG_MAX - IMAG_MIN )/ IMG_HEIGHT;

    for( rowIndex = 0; rowIndex < IMG_HEIGHT; rowIndex++ )
    {
        //cout << "ROW: " << rowIndex << endl;

        for( colIndex = 0; colIndex < IMG_WIDTH; colIndex++ )
        {
            //cout << "COL: " << colIndex << endl;
            
            c.real = REAL_MIN + ((float) rowIndex + scale_real);
            c.imag = IMAG_MIN + ((float) colIndex + scale_imag);

            colors[rowIndex][colIndex] = cal_pixel(c);
            cout << colors[rowIndex][colIndex] << " ";
        }

        cout << endl;
    }
   
    pim_write_black_and_white(fPtr, w, h, (const unsigned char**) colors);
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
