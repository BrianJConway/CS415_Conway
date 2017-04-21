/******************************************************************************
* FILE: parallel_matrixmultiplication.cpp
* DESCRIPTION:
*   Program that muliplies matrices.
*   The numbers are read in from a file, the first number denotes the number of
*   items to be sorted. This is the parallel implementation of this program.
******************************************************************************/
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <cstdio>
#include <vector>
#include <mpi.h>
#include <cmath>
#include "matrix_funcs.h"

#define WRITE_MATRIX 1 // write sorted elements to file (1), do not write (0)
#define TRIALS 0 // run program for all meta data files (1), run program once (0)

using namespace std;

// main function
int main(int argc, char *argv[])
{
  // initialization
  vector<vector<int>> matrixA;
  vector<vector<int>> matrixB;
  vector<vector<int>> matrixC;
  unsigned int n;
  int numtasks, taskid;
  char logpath[] = "../bin/log_parallel.csv";
  vector<double> trialTimes;
  // initialize MPI world
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
  MPI_Comm_rank(MPI_COMM_WORLD, &taskid);
  // run several trials
  #if TRIALS
  int trialIndex;
  for(trialIndex = 0; trialIndex < NUM_TRIALS; trialIndex++)
  {
  matrixA.clear();
  matrixB.clear();
  matrixC.clear();
  n = 0;
  #endif
  if(taskid == MASTER)
  {
    // initialize variables
    double starttime, endtime;
    vector<vector<int>> myMatrixA, myMatrixB, myMatrixC;
    int myN;
    // read in unsorted list
    if(!readMatrices(matrixA, matrixB, n))
    {
      cout << "Reading in data failed." << endl;
      return 1;
    }
    initializeMatrix(matrixC, n);
    // skew matrices
    skewMatrices(matrixA, matrixB, n);
    myN = recvMatrix(myMatrixA, myMatrixB);
    // start timing
    starttime = MPI_Wtime();
    calcAndShift(myMatrixA, myMatrixB, myMatrixC, myN);
    // end timing
    endtime = MPI_Wtime();
    trialTimes.push_back(endtime-starttime);
    // display computation time
    cout << "Multiplied " << n << "x" << n << " matrix in "
      << endtime-starttime << " seconds." << endl;
    #if WRITE_MATRIX // skip receiving result if not writing matrix
    recvMatrices(matrixC, myMatrixC, n);
    // write matrix to file
    writeMatrix(matrixC, n);
    #endif
  }
  // child processes
  else
  {
    // receive matrix, do calculations
    n = recvMatrix(matrixA, matrixB);
    calcAndShift(matrixA, matrixB, matrixC, n);
    #if WRITE_MATRIX // skip sending result if not writing matrix
    sendMatrix(matrixC, n);
    #endif
  }
  #if TRIALS
  }
  #endif
  if(taskid == MASTER)
  {
    // average caluculated trial times and return
    averageTrials(trialTimes, 1, logpath, n, TRIALS);
  }
  MPI_Finalize();
  return 0;
}
