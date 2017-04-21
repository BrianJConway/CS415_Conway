#include <cstring>
#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <cmath>
#include "matrix_funcs.h"

using namespace std;

// function implementations
bool readMatrices(vector<vector<int>> & matrixA, vector<vector<int>> & matrixB,
  unsigned int & n)
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
  filestream >> n;
  initializeMatrix(matrixA, n);
  initializeMatrix(matrixB, n);
  // read in first matrix
  for(rowIndex = 0; rowIndex < n; rowIndex++)
  {
    for(colIndex = 0; colIndex < n; colIndex++)
    {
      filestream >> matrixA[rowIndex][colIndex];
    }
  }
  // read in second matrix
  for(rowIndex = 0; rowIndex < n; rowIndex++)
  {
    for(colIndex = 0; colIndex < n; colIndex++)
    {
      filestream >> matrixB[rowIndex][colIndex];
    }
  }
  // check for failure
  if(filestream.fail() || n <= 0 )
  {
    return false;
  }
  filestream.close();
  return true;
}

// size matrix to n x n and zero all values
void initializeMatrix(vector<vector<int>> & matrix, const unsigned int n)
{
  unsigned int rowIndex, colIndex;
  matrix.resize(n);
  for(rowIndex = 0; rowIndex < n; rowIndex++)
  {
    matrix[rowIndex].resize(n);
    for(colIndex = 0; colIndex < n; colIndex++)
    {
      matrix[rowIndex][colIndex] = 0;
    }
  }
}

// multiply matrices
void multiplyMatrices(const vector<vector<int>> & matrixA,
  const vector<vector<int>> & matrixB, vector<vector<int>> & matrixC,
  const unsigned int n)
{
  unsigned int rowIndex, colIndex, index;
  for(rowIndex = 0; rowIndex < n; rowIndex++)
  {
    for(colIndex = 0; colIndex < n; colIndex++)
    {
      for(index = 0; index < n; index++)
      matrixC[rowIndex][colIndex] += (matrixA[rowIndex][index]*matrixB[index][colIndex]);
    }
  }
}

void skewMatrices(vector<vector<int>> & matrixA, vector<vector<int>> & matrixB,
  unsigned int n)
{
  // initialization
  int numtasks, taskIndex;
  MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
  MPI_Request request;
  unsigned int pSqrt = (unsigned int) sqrt(numtasks);
  unsigned int chunk = n / pSqrt;
  unsigned int i, j;
  vector<int> tempA;
  vector<int> tempB;
  tempA.resize(chunk);
  tempB.resize(chunk);
  // send chunk to each process
  for(taskIndex = 0; taskIndex < numtasks; taskIndex++)
  {
    MPI_Isend(&n, 1, MPI_INT, taskIndex, MSG_TAG, MPI_COMM_WORLD, &request);
    //MPI_Send(&n, 1, MPI_INT, taskIndex, MSG_TAG, MPI_COMM_WORLD);
    // send each row of each chunk
    for(i = 0; i < chunk; i++)
    {
      for(j = 0; j < chunk; j++)
      {
        tempA[j] = matrixA[taskIndex/pSqrt*chunk+i][((((taskIndex/pSqrt)+taskIndex)%pSqrt)*chunk)+j];
        tempB[j] = matrixB[(((taskIndex/pSqrt)+(taskIndex%pSqrt))%pSqrt)*chunk+i][(taskIndex%pSqrt*chunk)+j];
      }
      //MPI_Isend(&matrixA[taskIndex/pSqrt*chunk+i][(((taskIndex/pSqrt)+taskIndex)%pSqrt)*chunk],
      //  chunk, MPI_INT, taskIndex, i, MPI_COMM_WORLD, &request);
      MPI_Isend(&tempA[0], chunk, MPI_INT, taskIndex, i, MPI_COMM_WORLD, &request);
      //MPI_Isend(&matrixB[(((taskIndex/pSqrt)+(taskIndex%pSqrt))%pSqrt)*chunk+i][taskIndex%pSqrt*chunk],
        //chunk, MPI_INT, taskIndex, i+(int)chunk, MPI_COMM_WORLD, &request);
      MPI_Isend(&tempB[0],chunk, MPI_INT, taskIndex, i+(int)chunk, MPI_COMM_WORLD, &request);
    }
  }
}

int recvMatrix(vector<vector<int>> & matrixA, vector<vector<int>> & matrixB)
{
  // initialization
  int numtasks, taskid;
  MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
  MPI_Comm_rank(MPI_COMM_WORLD, &taskid);
  MPI_Status status;
  unsigned int n, chunk;
  unsigned int pSqrt = (unsigned int) sqrt(numtasks);
  unsigned int i;
  // prepare matrices
  MPI_Recv(&n, 1, MPI_INT, MASTER, MSG_TAG, MPI_COMM_WORLD, &status);
  chunk = n / pSqrt;
  initializeMatrix(matrixA, chunk);
  initializeMatrix(matrixB, chunk);
  // receive each row of each matrix
  for(i = 0; i < chunk; i++)
  {
    MPI_Recv(&matrixA[i][0], chunk, MPI_INT, MASTER, i, MPI_COMM_WORLD, &status);
    MPI_Recv(&matrixB[i][0], chunk, MPI_INT, MASTER, i+(int)chunk,
      MPI_COMM_WORLD, &status);
  }
  MPI_Barrier(MPI_COMM_WORLD);
  return n;
}

// do matrix calculation and rotate
void calcAndShift(vector<vector<int>> & matrixA, vector<vector<int>> & matrixB,
  vector<vector<int>> & matrixC, const unsigned int n)
{
  // initialization
  int numtasks, taskid;
  int sendATo, recvAFrom, sendBTo, recvBFrom;
  MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
  MPI_Comm_rank(MPI_COMM_WORLD, &taskid);
  vector<MPI_Request> requestA;
  vector<MPI_Request> requestB;
  MPI_Status status;
  unsigned int pSqrt = (unsigned int) sqrt(numtasks);
  unsigned int chunk = n / pSqrt;
  unsigned int i, rotationIndex;
  vector<vector<int>> bufferA, bufferB;
  calculateDestinations(taskid, (int) pSqrt, sendATo, recvAFrom, sendBTo, recvBFrom);
  // prepare matrices
  initializeMatrix(matrixC, chunk);
  initializeMatrix(bufferA, chunk);
  initializeMatrix(bufferB, chunk);
  requestA.resize(chunk);
  requestB.resize(chunk);
  // caluculate and rotate
  for(rotationIndex = 0; rotationIndex < pSqrt; rotationIndex++)
  {
    multiplyMatrices(matrixA, matrixB, matrixC, chunk);
    if(rotationIndex < pSqrt-1)
    {
      // send each row of shifted matrices
      for(i = 0; i < chunk; i++)
      {
        MPI_Isend(&matrixA[i][0], chunk, MPI_INT, sendATo, i, MPI_COMM_WORLD, &requestA[i]);
        MPI_Isend(&matrixB[i][0], chunk, MPI_INT, sendBTo, i+(int)chunk, MPI_COMM_WORLD, &requestB[i]);
      }
      // receive each row of shifted matrices
      for(i = 0; i < chunk; i++)
      {
        MPI_Irecv(&bufferA[i][0], chunk, MPI_INT, recvAFrom, MPI_ANY_TAG, MPI_COMM_WORLD, &requestA[i]);
        MPI_Irecv(&bufferB[i][0], chunk, MPI_INT, recvBFrom, MPI_ANY_TAG, MPI_COMM_WORLD, &requestB[i]);
      }
      // copy each row of from buffer to matrices
      for(i = 0; i < chunk; i++)
      {
        MPI_Wait(&requestA[i], &status);
        matrixA[status.MPI_TAG]=bufferA[i];
        bufferA[i].clear();
        MPI_Wait(&requestB[i], &status);
        matrixB[status.MPI_TAG-(int)chunk]=bufferB[i];
        bufferB[i].clear();
      }
    }
  }
  MPI_Barrier(MPI_COMM_WORLD);
}

void sendMatrix(vector<vector<int>> & matrixC, const unsigned int n)
{
  // initialization
  int numtasks;
  MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
  unsigned int pSqrt = (unsigned int) sqrt(numtasks);
  unsigned int chunk = n / pSqrt;
  unsigned int i;
  // send each row of each chunk to master
  for(i = 0; i < chunk; i++)
  {
    MPI_Send(&matrixC[i][0], chunk, MPI_INT, MASTER, i, MPI_COMM_WORLD);
  }
}

// master receives final calculations
void recvMatrices(vector<vector<int>> & matrixFinal, vector<vector<int>> & matrixC,
  const unsigned int n)
{
  // initialization
  int numtasks, taskIndex, taskid;
  MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
  MPI_Comm_rank(MPI_COMM_WORLD, &taskid);
  MPI_Status status;
  unsigned int pSqrt = (unsigned int) sqrt(numtasks);
  unsigned int chunk = n / pSqrt;
  unsigned int i, j;
  vector<int> buffer;
  buffer.resize(chunk);
  // save my matrix values
  for(i = 0; i < chunk; i++)
  {
    for(j = 0; j < chunk; j++)
    {
      matrixFinal[i][j] = matrixC[i][j];
    }
  }
  // recv chunk from each process
  for(taskIndex = 1; taskIndex < numtasks; taskIndex++)
  {
    // recv each row of each chunk
    for(i = 0; i < chunk; i++)
    {
      MPI_Recv(&buffer[0], chunk, MPI_INT, taskIndex, i, MPI_COMM_WORLD, &status);
      for(j = 0; j < chunk; j++)
      {
        matrixFinal[taskIndex/pSqrt*chunk+i][(taskIndex%pSqrt)*chunk+j] = buffer[j];
      }
    }
  }
}

// calculate send and receive destinations
void calculateDestinations(const int taskid, const int pSqrt, int &sendATo,
  int & recvAFrom, int & sendBTo, int & recvBFrom)
{
  if((taskid+1)%pSqrt == 0)
  {
    recvAFrom = taskid + 1 - pSqrt;
  }
  else
  {
    recvAFrom = taskid + 1;
  }
  if(taskid%pSqrt == 0)
  {
    sendATo = taskid - 1 + pSqrt;
  }
  else
  {
    sendATo = taskid - 1;
  }
  if(taskid/pSqrt == pSqrt - 1)
  {
    recvBFrom = taskid - ((pSqrt - 1)*(pSqrt));
  }
  else
  {
    recvBFrom = taskid + pSqrt;
  }
  if(taskid/pSqrt == 0)
  {
    sendBTo = taskid + ((pSqrt - 1)*(pSqrt));
  }
  else
  {
    sendBTo = taskid - pSqrt;
  }
}

// write matrix to file
void writeMatrix(const vector<vector<int>> & matrix, const unsigned int n)
{
  ofstream filestream;
  unsigned int rowIndex, colIndex;
  filestream.open(MATRIXPATH);
  for(rowIndex = 0; rowIndex < n; rowIndex++)
  {
    for(colIndex = 0; colIndex < n; colIndex++)
    {
      filestream << matrix[rowIndex][colIndex] << "\t";
    }
    filestream << endl;
  }
  filestream.close();
}

// print matrix to console
void printMatrix(const vector<vector<int>> & matrix, const unsigned int n)
{
  unsigned int rowIndex, colIndex;
  for(rowIndex = 0; rowIndex < n; rowIndex++)
  {
    for(colIndex = 0; colIndex < n; colIndex++)
    {
      cout << matrix[rowIndex][colIndex] << "\t";
    }
    cout << endl;
  }
  cout << endl;
}

// average the calculated trial times
void averageTrials(vector<double> trialTimes, const int tasks,
  const char logpath[], const unsigned int n, bool trials)
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
  logfile << tasks << ',' << n << ',' << averageTrial << endl;
  logfile.close();
}
