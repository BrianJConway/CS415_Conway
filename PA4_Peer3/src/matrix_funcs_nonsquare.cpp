#include <cstring>
#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include "matrix_funcs_nonsquare.h"

using namespace std;

// function implementations
bool readMatrices(vector<vector<int>> & matrixA, vector<vector<int>> & matrixB,
  Dimensions &dim)
{
  // initialization
  ifstream filestream;
  unsigned int rowIndex, colIndex;
  // open file
  filestream.open(FILEPATH, fstream::in);
  if(!filestream.good())
  {
    return false;
  }
  // get matrix sizes from file
  filestream >> dim.n >> dim.m >> dim.p;
  matrixA.resize(dim.n);
  matrixB.resize(dim.p);
  // read in first matrix
  for(rowIndex = 0; rowIndex < dim.n; rowIndex++)
  {
    matrixA[rowIndex].resize(dim.m);
    for(colIndex = 0; colIndex < dim.m; colIndex++)
    {
      filestream >> matrixA[rowIndex][colIndex];
    }
  }
  // read in second matrix
  for(rowIndex = 0; rowIndex < dim.p; rowIndex++)
  {
    matrixB[rowIndex].resize(dim.m);
  }
  for(colIndex = 0; colIndex < dim.m; colIndex++)
  {
    for(rowIndex = 0; rowIndex < dim.p; rowIndex++)
    {
      filestream >> matrixB[rowIndex][colIndex];
    }
  }
  // check for failure
  if(filestream.fail() || dim.n <= 0 || dim.m <= 0 || dim.p <= 0)
  {
    return false;
  }
  filestream.close();
  return true;
}

// multiply matrices
void multiplyMatrices(const vector<vector<int>> & matrixA,
  const vector<vector<int>> & matrixB, vector<vector<int>> & matrixC,
  const Dimensions dim)
{
  unsigned int rowIndex, colIndex;
  matrixC.resize(dim.n);
  for(rowIndex = 0; rowIndex < dim.n; rowIndex++)
  {
    matrixC[rowIndex].resize(dim.p);
    for(colIndex = 0; colIndex < dim.p; colIndex++)
    {
      matrixC[rowIndex][colIndex] = matrixCalculation(matrixA[rowIndex], matrixB[colIndex], dim);
    }
  }
}

// multiply a matrixA row and a matrixB column to get a matrixC value
int matrixCalculation(const vector<int> &rowA, const vector<int> &colB,
  const Dimensions dim)
{
  unsigned int index;
  int value = 0;
  for(index = 0; index < dim.m; index++)
  {
    value += (rowA[index] * colB[index]);
  }
  return value;
}

// write matrix to file
void writeMatrix(const vector<vector<int>> & matrixC, const Dimensions dim)
{
  ofstream filestream;
  unsigned int rowIndex, colIndex;
  filestream.open(MATRIXPATH);
  for(rowIndex = 0; rowIndex < dim.n; rowIndex++)
  {
    for(colIndex = 0; colIndex < dim.p; colIndex++)
    {
      filestream << matrixC[rowIndex][colIndex] << "\t";
    }
    filestream << endl;
  }
  filestream.close();
}

// average the calculated trial times
void averageTrials(vector<double> trialTimes, const int tasks,
  const char logpath[], const Dimensions dim, bool trials)
{
  // initialization
  double averageTrial = trialTimes[0];
  if(trials)
  {
    int index;
    sort(trialTimes.begin(), trialTimes.end());
    averageTrial = trialTimes[trialTimes.size()/2];
    // calculate average if 3 or more trials
    if( NUM_TRIALS >= 3 )
    {
      averageTrial = 0;
      // disregaurd minimum and maximum
      for(index = 1; index < NUM_TRIALS - 1; index++)
      {
        averageTrial += trialTimes[index];
      }
      averageTrial = averageTrial/(NUM_TRIALS-2);
    }
  }
  // write average, number of tasks, and image size to file
  ofstream logfile;
  logfile.open(logpath, ios_base::app);
  logfile << tasks << ',' << dim.n << ',' << dim.m << ',' << dim.p << ',' << averageTrial << endl;
  logfile.close();
}
