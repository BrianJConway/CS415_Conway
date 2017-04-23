/**
 *
 * PA4: Parallel Matrix Multiplication
 * Name: [REDACTED]
 * Date: 04/20/2017
 * Description:
 * Performs Cannon's Algorithm to do matrix
 * multiplcation.
 *
 **/
#include <iostream>
#include <cstdlib>
#include <mpi.h>
#include <cmath>
using namespace std;

int** allocateSquareMatrix( int size );
int** matrixMultiplySeq( int** A, int** B, int size );
int** subMatrix( int** matrix, int x, int y, int subSize );
void fillMatrix( int** matrix, int dimensions );
void zeroMatrix( int** matrix, int dimensions );
void printMatrix( int** matrix, int dimensions );
int** addMatrices( int** A, int** B, int dimensions );

#define DIST_A 1
#define DIST_B 2
#define SHIFT_A 3
#define SHIFT_B 4

int main( int argc, char** argv )
{
	// declare variables and initialize MPI
	int num_tasks, task_id, size;
	double start_time, end_time;
	MPI_Init( &argc, &argv );
	MPI_Comm_size( MPI_COMM_WORLD, &num_tasks );
	MPI_Comm_rank( MPI_COMM_WORLD, &task_id );

	// Verify that program has correct number of
	// arguments.
	if( argc < 2 || argc > 2 )
	{
		if( task_id == 0 )
		{
			cout << "Usage: main [size]" << endl;
		}
		MPI_Finalize( );
		return 1;
	}

	// Process arguments
	size = atoi(argv[1]);

	// If only one task, run sequential
	if( num_tasks == 1 )
	{
		// Initialize Matrices
		int** A = allocateSquareMatrix( size );
		int** B = allocateSquareMatrix( size );
		fillMatrix( A, size );
		fillMatrix( B, size );

		// Preform multiplicaton
		start_time = MPI_Wtime( );
		int ** result = matrixMultiplySeq( A, B, size );
		end_time = MPI_Wtime( );
		
		// printMatrix( result, size );
	}
	// Otherwise, run parallel
	else
	{
		int grid_size, sub_dim, sub_size;
		int reorder, ierr, dim_size[2], periods[2];

		int** A;
		int** B;
		int** C;
		// Verify square number of processors.
		if( ( (int) sqrt( num_tasks ) ) != sqrt( num_tasks ) )
		{
			if( task_id == 0 )
			{
				cout << "Invalid number of tasks." << endl;
				cout << "Must be a perfect square." << endl;
			}
			MPI_Finalize( );
			return 1; 
		}
		grid_size = (int) sqrt( num_tasks );
		// Verify rows are appropriate.
		if( size % grid_size != 0 )
		{
			if( task_id == 0 )
			{
				cout << "Data size must be divisible by the dimensions of the grid." << endl;
				cout << "(i.e. " << size << " mod " << grid_size << " is not 0)" << endl;
			}
			MPI_Finalize( );
			return 1;
		}

		sub_dim = size / grid_size;
		sub_size = sub_dim * sub_dim;
		A = allocateSquareMatrix( sub_dim );
		B = allocateSquareMatrix( sub_dim );
		C = allocateSquareMatrix( sub_dim );
		zeroMatrix( C, sub_dim );

		// Create Grid Layout for MPI Tasks.  Makes moving
		// submatricies logically easier.
		MPI_Comm GRID_COMM;
		MPI_Comm OLD_COMM = MPI_COMM_WORLD;
		dim_size[0] = grid_size;
		dim_size[1] = grid_size;
		periods[0] = true;
		periods[1] = true;
		reorder = false;

		ierr =  MPI_Cart_create( OLD_COMM, 2, dim_size,
								 periods, reorder, &GRID_COMM );
		
		// Locate myself in the grid.
		int myCoords[2];
		MPI_Cart_coords( GRID_COMM, task_id, 2, myCoords );

		// If master task Allocate and distribute sub matrices.
		if( task_id == 0 )
		{
			// Allocate and fill Matrices
			int** masterA = allocateSquareMatrix( size );
			int** masterB = allocateSquareMatrix( size );
			fillMatrix( masterA, size );
			fillMatrix( masterB, size );

			// Obtain master's submatrix
			A = subMatrix( masterA, myCoords[0] * sub_dim, myCoords[1] * sub_dim, sub_dim );
			B = subMatrix( masterB, myCoords[0] * sub_dim, myCoords[1] * sub_dim, sub_dim );
			MPI_Request requestsA[num_tasks];
			MPI_Request requestsB[num_tasks];
			
			// Send out submatrices to other tasks
			for( int i = 0; i < grid_size; i++ )
			{
				for( int j = 0; j < grid_size; j++ )
				{
					// obtain rank based on grid position
					int recv_rank;
					int recv_coords[2];
					recv_coords[0] = i; recv_coords[1] = j;
					MPI_Cart_rank( GRID_COMM, recv_coords, &recv_rank );
					
					// If that rank is not 0, send out the 
					// appropriate submatrix.
					if( recv_rank != task_id )
					{
						int** buffA = subMatrix( masterA, i * sub_dim, j * sub_dim, sub_dim );
						int** buffB = subMatrix( masterB, i * sub_dim, j * sub_dim, sub_dim );
						MPI_Isend( &buffA[0][0], sub_size, MPI_INT, recv_rank, DIST_A, MPI_COMM_WORLD, &requestsA[recv_rank] );
						MPI_Isend( &buffB[0][0], sub_size, MPI_INT, recv_rank, DIST_B, MPI_COMM_WORLD, &requestsB[recv_rank] );
					}
				}
			}
			// Wait for all of the receives to complete
			// on the slaves.
			for( int i = 1; i < num_tasks; i++ )
			{
				MPI_Status wait_status;
				MPI_Wait( &requestsA[i], &wait_status );
				MPI_Wait( &requestsB[i], &wait_status );
			}
			
			// Wait until all tasks are here.
			MPI_Barrier( MPI_COMM_WORLD );
		}
		// If not the master
		else
		{
			// Receive the two sub matrices.
			MPI_Status recv_status;
			MPI_Recv( &A[0][0], sub_size, MPI_INT, 0, DIST_A, MPI_COMM_WORLD, &recv_status );
			MPI_Recv( &B[0][0], sub_size, MPI_INT, 0, DIST_B, MPI_COMM_WORLD, &recv_status );
			
			// Wait until all tasks are here.
			MPI_Barrier( MPI_COMM_WORLD );
		}

		// Start the timer
		start_time = MPI_Wtime( );

		// Initialize Matrix
		int send_rank, recv_rank;
		int send_coords[2], recv_coords[2];
		MPI_Status recv_status;
		// Shift A Left
		if( myCoords[0] != 0 )
		{
			// Calculate coordinates of task to send to
			// and task to receive from.
			send_coords[0] = myCoords[0];
			send_coords[1] = myCoords[1] - myCoords[0];
			recv_coords[0] = myCoords[0];
			recv_coords[1] = myCoords[1] + myCoords[0]; 

			// Obtain rank of task to send to and
			// task to receive from.
			MPI_Cart_rank( GRID_COMM, send_coords, &send_rank );
			MPI_Cart_rank( GRID_COMM, recv_coords, &recv_rank );
			
			// Send to the send rank, and then receive
			// and replace my old A.
			MPI_Sendrecv_replace( &A[0][0], sub_size, MPI_INT,
								  send_rank, SHIFT_A,
								  recv_rank, SHIFT_A,
								  MPI_COMM_WORLD, &recv_status);
		}
		// Shift B Up.
		if( myCoords[1] != 0 )
		{
			// Calculate coordinates of task to send to
			// and task to receive from
			send_coords[1] = myCoords[1];
			send_coords[0] = myCoords[0] - myCoords[1];
			recv_coords[1] = myCoords[1];
			recv_coords[0] = myCoords[0] + myCoords[1]; 

			// Obtain rank of task to send to and
			// task to receive from.
			MPI_Cart_rank( GRID_COMM, send_coords, &send_rank );
			MPI_Cart_rank( GRID_COMM, recv_coords, &recv_rank );
			
			// Send to the send rank, and then receive
			// and replace my old B.
			MPI_Sendrecv_replace( &B[0][0], sub_size, MPI_INT,
								  send_rank, SHIFT_B,
								  recv_rank, SHIFT_B,
								  MPI_COMM_WORLD, &recv_status);
		}

		// Wait until all tasks are here.
		MPI_Barrier( MPI_COMM_WORLD );

		// Multiply and shift.
		for( int i = 0; i < grid_size; i++ )
		{
			// Multiply and Accumulate
			int** temp = matrixMultiplySeq( A, B, sub_dim );
			C = addMatrices( C, temp, sub_dim );
			
			// shift A
			// Calculate coordinates of task to send to
			// and task to receive from
			send_coords[0] = myCoords[0];
			send_coords[1] = myCoords[1] - 1;
			recv_coords[0] = myCoords[0];
			recv_coords[1] = myCoords[1] + 1;
			
			// Obtain rank of task to send to and
			// task to receive from.
			MPI_Cart_rank( GRID_COMM, send_coords, &send_rank );
			MPI_Cart_rank( GRID_COMM, recv_coords, &recv_rank );
			
			// Send to the send rank, and then receive
			// and replace my old A.
			MPI_Sendrecv_replace( &A[0][0], sub_size, MPI_INT,
								  send_rank, SHIFT_A,
								  recv_rank, SHIFT_A,
								  MPI_COMM_WORLD, &recv_status);
			
			// shift B
			// Calculate coordinates of task to send to
			// and task to receive from
			send_coords[1] = myCoords[1];
			send_coords[0] = myCoords[0] - 1;
			recv_coords[1] = myCoords[1];
			recv_coords[0] = myCoords[0] + 1; 

			// Obtain rank of task to send to and
			// task to receive from.
			MPI_Cart_rank( GRID_COMM, send_coords, &send_rank );
			MPI_Cart_rank( GRID_COMM, recv_coords, &recv_rank );
			
			// Send to the send rank, and then receive
			// and replace my old B.
			MPI_Sendrecv_replace( &B[0][0], sub_size, MPI_INT,
								  send_rank, SHIFT_B,
								  recv_rank, SHIFT_B,
								  MPI_COMM_WORLD, &recv_status);
			
			// Wait until all tasks are here.
			MPI_Barrier( MPI_COMM_WORLD );
		}
		
		// stop timer
		end_time = MPI_Wtime( );
	}

	// Print results
	if( task_id == 0 )
	{
		printf( "%d,%d,%.6f\n", num_tasks, size, end_time - start_time );
	}
	
	// Close out.
	MPI_Finalize( );
	return 0;
}

// Allocates a size x size matrix.
// This matrix is made up of contiguous memory
// and is therefore suitable for sending /
// receiving with MPI.
int** allocateSquareMatrix( int size )
{
	int *data = (int *) malloc( size * size * sizeof( int ) );
    int **array= (int **) malloc( size * sizeof( int* ) );
    for ( int i = 0; i < size; i++ )
	{
        array[i] = &( data[size * i] );
	}

    return array;
}

// Sequential matrix multiplication.
int** matrixMultiplySeq( int** A, int** B, int size )
{
	int i, j, k, sum;
	int** result = allocateSquareMatrix( size );

	for( i = 0; i < size; i++ )
	{
		for( j = 0; j < size; j++ )
		{
			sum = 0;
			for( k = 0; k < size; k++ )
			{
				sum = sum + A[j][k] * B[k][i];
			}
			result[i][j] = sum;
		}
	}

	return result;
}

// Returns a subSize x subSize matrix that is a
// submatrix of matrix.  The upper left corner
// of the matrix is denoted by x, y. Does not
// currently ensure that the request is in-
// bounds.
int** subMatrix( int** matrix, int x, int y, int subSize )
{
	int** result = allocateSquareMatrix( subSize );
	for( int i = 0; i < subSize; i++ )
	{
		for( int j = 0; j < subSize; j++ )
		{
			result[i][j] = matrix[y+i][x+j];
		}
	}
	return result;
}


// Fills matrix with values for testing
void fillMatrix( int** matrix, int dimensions )
{
	for( int i = 0; i < dimensions; i++ )
	{
		for( int j = 0; j < dimensions; j++ )
		{
			matrix[i][j] = i + j + 1;
		}
	}
}

// Fills matrix with zeros.
void zeroMatrix( int** matrix, int dimensions )
{
	for( int i = 0; i < dimensions; i++ )
	{
		for( int j = 0; j < dimensions; j++ )
		{
			matrix[i][j] = 0;
		}
	}
}

// Prints matrix
void printMatrix( int** matrix, int dimensions )
{
	for( int i = 0; i < dimensions; i++ )
	{
		cout << "["; 
		for( int j = 0; j < dimensions; j++ )
		{
			cout << " " << matrix[i][j];
		}
		cout << "]" << endl;
	}
}

// Adds two matrices and returns the result.
int** addMatrices( int** A, int** B, int dimensions )
{
	int** result = allocateSquareMatrix( dimensions );
	for( int i = 0; i < dimensions; i++ )
	{
		for( int j = 0; j < dimensions; j++ )
		{
			result[i][j] = A[i][j] + B[i][j];
		}
	}
	return result;
}