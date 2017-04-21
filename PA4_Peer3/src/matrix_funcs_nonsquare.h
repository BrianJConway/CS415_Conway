#ifndef __FUNCS_H__
#define __FUNCS_H__

#include <vector>
#include <mpi.h>

using namespace std;

// global constants
const char FILEPATH[] = "../bin/dataset.txt";
const char MATRIXPATH[] = "../bin/matrix.txt";
const int MASTER = 0;
const int NUM_TRIALS = 5;
const int MSG_TAG = 99;

// structs
struct Dimensions
{
  unsigned int n;
  unsigned int m;
  unsigned int p;
};

// function declarations
bool readMatrices(vector<vector<int>> & matrixA, vector<vector<int>> & matrixB,
  Dimensions &dim);
void multiplyMatrices(const vector<vector<int>> & matrixA,
  const vector<vector<int>> & matrixB, vector<vector<int>> & matrixC,
  const Dimensions dim);
int matrixCalculation(const vector<int> &rowA, const vector<int> &colB,
  const Dimensions dim);
void writeMatrix(const vector<vector<int>> & matrixC, const Dimensions dim);
void averageTrials(vector<double> trialTimes, const int tasks,
  const char logpath[], const Dimensions dim, bool trials);
#endif
