/******************************************************************************
* FILE: sequential_matrixmultiplication_nonsquare.cpp
* DESCRIPTION:
*   Program that muliplies matrices.
*   The numbers are read in from a file, the first number denotes the number of
*   items to be sorted. This is the sequential implementation of this program.
******************************************************************************/
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <cstdio>
#include <vector>
#include <mpi.h>
#include "matrix_funcs_nonsquare.h"

#define WRITE_MATRIX 1 // write sorted elements to file (1), do not write (0)
#define TRIALS 0 // run program for all meta data files (1), run program once (0)

using namespace std;

// main function
int main(int argc, char *argv[])
{
  // initialize variables
  int numtasks, taskid, len;
  Dimensions dim;
  char hostname[MPI_MAX_PROCESSOR_NAME];
  double starttime, endtime;
  char logpath[] = "../bin/log_sequential_nonsquare.csv";
  vector<vector<int>> matrixA;
  vector<vector<int>> matrixB;
  vector<vector<int>> matrixC;
  vector<double> trialTimes;
  // initialize MPI world
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
  MPI_Comm_rank(MPI_COMM_WORLD, &taskid);
  MPI_Get_processor_name(hostname, &len);
  // run several trials
  #if TRIALS
  int trialIndex;
  for(trialIndex = 0; trialIndex < NUM_TRIALS; trialIndex++)
  {
  matrixA.clear();
  matrixB.clear();
  matrixC.clear();
  #endif
  // read in unsorted list
  if(!readMatrices(matrixA, matrixB, dim))
  {
    cout << "Reading in data failed." << endl;
    return 1;
  }
  // start timing
  starttime = MPI_Wtime();
  multiplyMatrices(matrixA, matrixB, matrixC, dim);
  // end timing
  endtime = MPI_Wtime();
  trialTimes.push_back(endtime-starttime);
  // display computation time
  cout << "Multiplied " << dim.n << "x" << dim.m << " with " << dim.m << "x"
    << dim.p << " matrix in " << endtime-starttime << " seconds." << endl;
  #if WRITE_MATRIX
  // print sorted list
  writeMatrix(matrixC, dim);
  #endif
  #if TRIALS
  }
  #endif
  // average caluculated trial times and return
  averageTrials(trialTimes, 1, logpath, dim, TRIALS);
  MPI_Finalize();
  return 0;
}
