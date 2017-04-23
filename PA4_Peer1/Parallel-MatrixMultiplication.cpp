// Included libraries
#include <iostream>
#include <fstream>
#include <cstring>
#include <stdlib.h>
#include <math.h>
#include "mpi.h"

using namespace std;

#define MAX_SIZE 10

const bool printResults = true;

// Data Structures
struct minMatrix
{
	int sCol;
	int sRow;
	int matrixDim;
	int * smallA;
	int * smallB;
	int * result;
};

// Function Declaration
void createArrays(int * &matrix, int dimension);
void fillArrays(int * &matrixA, int * &matrixB, int dimension);
bool testParameters(int numtasks, int dimension, int &matrixDim);
void sendSmallMatrix(int * &matrix1, int * &matrix2, int dimension, minMatrix subMatrix);
void matrixMultiplication(int dimension, int * matrixA, int * matrixB, int * matrixC, MPI_Comm comm);
void multiply(int dimension, int * matrixA, int * matrixB, int * matrixC);
void gatherResults(int * &results, int dimension, minMatrix subMatrix);
void printOutput(int * &matrix, char matrixName, int dimension);

int main( int argc, char *argv[] )
{
	// Variables
	ofstream fout;
	char a = 'A';
	char b = 'B';
	char c = 'C';
	int dimension;
	int count;
	int * matrix1;
	int * matrix2;
	int * resultMatrix;
	minMatrix subMatrix;
	int numtasks;
	int taskid;
	double start = 0;
	double end = 0;

	// Test to see if Data file was Included
	if(argc != 2)
	{
		cout << "Please include size of the matrices" << endl;
		return 0;
	}

	// Initialize MPI World for timer
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
  	MPI_Comm_rank(MPI_COMM_WORLD,&taskid);

	// Setup size of both matrices
	dimension = atoi(argv[1]);

	// Test to see if process can do the Multiplication; otherwise initialize subMatrix
	if(testParameters(numtasks, dimension, subMatrix.matrixDim))
	{
		// Set up subMatrix data structure
		subMatrix.sCol = 0;
		subMatrix.sRow = 0;
		createArrays(subMatrix.smallA, subMatrix.matrixDim);
		createArrays(subMatrix.smallB, subMatrix.matrixDim);
		createArrays(subMatrix.result, subMatrix.matrixDim);

		for(count = 0; count < subMatrix.matrixDim * subMatrix.matrixDim; count++)
		{
				subMatrix.result[count] = 0;
		}

		// Set starting values for each processes
		for(count = 0; count < taskid; count++)
		{
			subMatrix.sCol += subMatrix.matrixDim;

			if(subMatrix.sCol == dimension)
			{
				subMatrix.sCol = 0;
				subMatrix.sRow += subMatrix.matrixDim;
			}
		}
	} else
	{
		// Close MPI
		MPI_Finalize();
		return 0;
	}

	// Create first two matrices
	createArrays(matrix1, dimension);
	createArrays(matrix2, dimension);

	// Have the master create the result matrices and fill the matrices A and B
	if(taskid == 0)
	{
		createArrays(resultMatrix, dimension);
		fillArrays(matrix1, matrix2, dimension);
	}

	// Broadcast the matrices to other processes
	MPI_Bcast(matrix1, dimension*dimension, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(matrix2, dimension*dimension, MPI_INT, 0, MPI_COMM_WORLD);

	// Split data up per processes
	sendSmallMatrix(matrix1, matrix2, dimension, subMatrix);

	MPI_Barrier(MPI_COMM_WORLD);

	// Start Timer
	if(taskid == 0)
	{
		start = MPI_Wtime();
	}

	// Start matrix multiplication
	matrixMultiplication(dimension, subMatrix.smallA, subMatrix.smallB, subMatrix.result, MPI_COMM_WORLD);

	MPI_Barrier(MPI_COMM_WORLD);

	// Stop Timer
	if(taskid == 0)
	{
		end = MPI_Wtime();
	}

	// Test to print out matrices
	if(printResults)
	{
		gatherResults(resultMatrix, dimension, subMatrix);

		if(taskid == 0)
		{
			printOutput(matrix1, a, dimension);
			printOutput(matrix2, b, dimension);
			printOutput(resultMatrix, c, dimension);
		}
	}

	// Close MPI
	MPI_Finalize();

	// Print out to a data file
	if(taskid == 0)
	{
		fout.open("Parallel-MatrixMultiplication.csv", ios::app);
		fout << dimension << "x" << dimension << ", ";
		fout << dimension << "x" << dimension << ", ";
		fout << dimension << "x" << dimension << ", ";
		fout << end - start << endl;
		fout.close();
	}

	return 0;
}

/*
	Function to create matrices
	Inputs: pointer, and the dimension of the matrix
	Output: None
*/
void createArrays(int * &matrix, int dimension)
{
	// Variables

	// Create array
	matrix = new int [dimension*dimension];
}

/*
	Function to fill matrix A and B with random numbers
	Inputs: pointer for both matrix A and B, and the dimension for both matrices
	Output: None
*/
void fillArrays(int * &matrixA, int * &matrixB, int dimension)
{
	// Variables
	int count;

	// Set srand
	srand (time(NULL));

	// Loop through each memory location
	for(count = 0; count < dimension*dimension; count++)
	{
		matrixA[count] = random() % MAX_SIZE;
		matrixB[count] = random() % MAX_SIZE;
	}
}

/*
	Function to test if given information is allowed
	Inputs: number of processes, the dimension of the matrix, and
			the small dimension of the sub matrixes for each processes
	Output: bool for pass for fail
*/
bool testParameters(int numtasks, int dimension, int &matrixDim)
{
	// Variables
	int root = sqrt(numtasks);
  int taskid;

  	// Get processes ID
  	MPI_Comm_rank(MPI_COMM_WORLD,&taskid);

	// Check if the numtasks is a perfect square
	if(numtasks != root * root)
	{
    if(taskid == 0)
    {
      cout << "The number of prcesses is not a perfect squeue - cannot perform matrix multiplication" << endl;
    }

    return false;
	}

	// Check if the height and width are a multiple of root
	if(dimension % root != 0)
	{
    if(taskid == 0)
    {
      cout << "The dimensions of matrices is not a multiple of the root of number of processes -  cannot perform matrix multiplication" << endl;
    }

    return false;
	}

	// Set Scales
	matrixDim = dimension/root;

	return true;
}

/*
	Function for each processor to gather all their
			data into their smaller matrices
	Inputs: pointer for both matrix A and B, and the dimension of the matrix, and the processors
			data structure
	Output: None
*/
void sendSmallMatrix(int * &matrix1, int * &matrix2, int dimension, minMatrix subMatrix)
{
	// Variables
	int count;
	int index = 0;
	int currentPos = subMatrix.sRow*dimension + subMatrix.sCol;
	int size = subMatrix.matrixDim*subMatrix.matrixDim;

	// Loop through each processes data
	for(count = 0; count < size; count++, currentPos++, index++)
	{
		subMatrix.smallA[count] = matrix1[currentPos];
		subMatrix.smallB[count] = matrix2[currentPos];

		if(index == subMatrix.matrixDim - 1)
		{
			currentPos += (dimension - index - 1);
			index = - 1;
		}
	}
}

/*
	Function that performs a matrix multiplication operation
	Inputs: dimension of the larger matrices, pointers to each processes sub matrices
	Output: None
*/
void matrixMultiplication(int dimension, int * matrixA, int * matrixB, int * matrixC, MPI_Comm comm)
{
   int count;
   int subMatrixDim;
   int numtasks;
   int dimsCT[2];
   int periods[2];
   int taskid;
   int CTtaskid;
   int mycoords[2];
   int upRank;
   int downRank;
   int leftRank;
   int rightRank;
   MPI_Comm CTcomm;

   // Get orginal MPI communtication information
   MPI_Comm_size(comm, &numtasks);
   MPI_Comm_rank(comm, &taskid);

	// Set up a cartesian topology
   dimsCT[0] = dimsCT[1] = sqrt(numtasks);
   subMatrixDim = dimension/dimsCT[0];
   periods[0] = periods[1] = 1;

	// Create a cartesian topology
	MPI_Cart_create(comm, 2, dimsCT, periods, false, &CTcomm);

	// Get the rank and coordinates for the new topology
	MPI_Comm_rank(CTcomm, &CTtaskid);
	MPI_Cart_coords(CTcomm, CTtaskid, 2, mycoords);

	// Find processes to the right, left, up and down
   MPI_Cart_shift(CTcomm, 1, 1, &leftRank, &rightRank);
   MPI_Cart_shift(CTcomm, 0, 1, &upRank, &downRank);

   // Set up matrices for multiplication
   for(count = 0; count < mycoords[0]; count++)
   {
	   MPI_Sendrecv_replace(matrixA, subMatrixDim*subMatrixDim, MPI_INT, leftRank,
		  1, rightRank, 1, CTcomm, MPI_STATUS_IGNORE);
   }

   for(count = 0; count < mycoords[1]; count++)
   {
	   MPI_Sendrecv_replace(matrixB, subMatrixDim*subMatrixDim, MPI_INT, upRank,
		  1, downRank, 1, CTcomm, MPI_STATUS_IGNORE);
   }

   // Multiply matrices together
   for (count = 0; count < dimsCT[0]; count++)
   {
	   multiply(subMatrixDim, matrixA, matrixB, matrixC);

	   MPI_Sendrecv_replace(matrixA, subMatrixDim*subMatrixDim, MPI_INT,
	   leftRank, 1, rightRank, 1, CTcomm, MPI_STATUS_IGNORE);

	   MPI_Sendrecv_replace(matrixB, subMatrixDim*subMatrixDim, MPI_INT,
	   upRank, 1, downRank, 1, CTcomm, MPI_STATUS_IGNORE);
   }

	// Free up new MPI comm world
	MPI_Comm_free(&CTcomm);
}

/*
	Function to multiply which processes sub matrix A and B
	Inputs: dimension of the sub matrices, pointers to all process matrices;
			A, B , and C
	Output: None
*/
void multiply(int dimension, int * matrixA, int * matrixB, int * matrixC)
{
	// Variables
	int count;
	int index;
	int spot;

	// Loop through both matrices
	for (count = 0; count < dimension; count++)
		for (index = 0; index < dimension; index++)
			for (spot = 0; spot < dimension; spot++)
			{
				matrixC[count * dimension + index] +=
					matrixA[count * dimension + spot] * matrixB[spot * dimension + index];
			}
 }

 /*
 	Function to gather all processes results for printing purposes
 	Inputs: pointer to matrix C, the dimensions of the matrix C, and the data structure
			for the processor
 	Output: None
 */
 void gatherResults(int * &results, int dimension, minMatrix subMatrix)
 {
   // Variables
   minMatrix tempResult;
   int count = 0;
   int index = 0;
 	int indexRow;
   int currentPos = 0;
 	int size = subMatrix.matrixDim*subMatrix.matrixDim;
   int taskid;
   int numtasks;

   // Get rank of process
   MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
   MPI_Comm_rank(MPI_COMM_WORLD,&taskid);

   // Set up tempResult for the master process and start copying data
   if(taskid == 0)
   {
     tempResult.matrixDim = subMatrix.matrixDim;
     createArrays(tempResult.result, size);

     // Loop through until each processes results have been recvieved
     while(count < numtasks - 1)
     {
       MPI_Recv( &tempResult.sRow, 1, MPI_INT, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE );
       MPI_Recv( &tempResult.sCol, 1, MPI_INT, MPI_ANY_SOURCE, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE );
       MPI_Recv( tempResult.result, size, MPI_INT, MPI_ANY_SOURCE, 3, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
       count++;
 			indexRow = 0;

       // Copy over the results
 			currentPos = tempResult.sRow*dimension + tempResult.sCol;

 			for(index = 0; index < size; index++, currentPos++, indexRow++)
 			{
         results[currentPos] = tempResult.result[index];

 				if(indexRow == tempResult.matrixDim - 1)
 				{
 					currentPos += (dimension - indexRow - 1);
 					indexRow = - 1;
 				}
       }
     }

 		// Copy over the results
 		currentPos = subMatrix.sRow*dimension + subMatrix.sCol*dimension;
 		indexRow = 0;

 		for(index = 0; index < size; index++, currentPos++, indexRow++)
 		{
 			results[currentPos] = subMatrix.result[index];

 			if(indexRow == subMatrix.matrixDim - 1)
 			{
 				currentPos += (dimension - indexRow - 1);
 				indexRow = - 1;
 			}
 		}
   } else
   {
     // Send master results
     MPI_Send( &subMatrix.sRow, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
     MPI_Send( &subMatrix.sCol, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
     MPI_Send( subMatrix.result, size, MPI_INT, 0, 3, MPI_COMM_WORLD);
   }
 }

 /*
 	Function to print out any giving matrice
 	Inputs: pointer to the given matrix, the matrixs name, and the dimension of the matrix
 	Output: None
 */
 void printOutput(int * &matrix, char matrixName, int dimension)
 {
 	// Variables
 	int count;
 	int size = dimension * dimension;
 	int index = 0;

 	// Print the matrix
 	cout << "Matrix " << matrixName << ":" << endl;

 	for(count = 0; count < size; count++, index++)
 	{
 		cout << matrix[count] << " ";

 		if(index == dimension - 1 )
 		{
 			cout << endl;
 			index = -1;
 		}
 	}

 	cout << endl;

 }
