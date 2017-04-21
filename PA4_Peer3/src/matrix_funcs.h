#ifndef __FUNCS_H__
#define __FUNCS_H__

#include <vector>
#include "mpi.h"

using namespace std;

// global constants
const char FILEPATH[] = "../bin/dataset.txt";
const char MATRIXPATH[] = "../bin/matrix.txt";
const int MASTER = 0;
const int NUM_TRIALS = 5;
const int SKEWA_TAG = 72;
const int SKEWB_TAG = 88;
const int MSG_TAG = 99;

// function declarations
bool readMatrices(vector<vector<int>> & matrixA, vector<vector<int>> & matrixB,
  unsigned int & n);
void initializeMatrix(vector<vector<int>> & matrix, const unsigned int n);
void multiplyMatrices(const vector<vector<int>> & matrixA,
  const vector<vector<int>> & matrixB, vector<vector<int>> & matrixC,
  const unsigned int n);
void skewMatrices(vector<vector<int>> & matrixA, vector<vector<int>> & matrixB,
  unsigned int n);
void calcAndShift(vector<vector<int>> & matrixA, vector<vector<int>> & matrixB,
  vector<vector<int>> & matrixC, const unsigned int n);
void sendMatrix(vector<vector<int>> & matrixC, const unsigned int n);
int recvMatrix(vector<vector<int>> & matrixA, vector<vector<int>> & matrixB);
void calculateDestinations(const int taskid, const int pSqrt, int &sendATo, int & recvAFrom, int & sendBTo, int & recvBFrom);
void recvMatrices(vector<vector<int>> & matrixFinal, vector<vector<int>> & matrixC, const unsigned int n);
void writeMatrix(const vector<vector<int>> & matrix, const unsigned int n);
void printMatrix(const vector<vector<int>> & matrix, const unsigned int n);
void averageTrials(vector<double> trialTimes, const int tasks,
  const char logpath[], const unsigned int n, bool trials);
#endif
