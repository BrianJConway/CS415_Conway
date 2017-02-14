/******************************************************************************
* FILE: mpi_ping.c
* DESCRIPTION:
*   MPI tutorial example code: Blocking Send/Receive
* AUTHOR: Blaise Barney
* LAST REVISED: 04/02/05
******************************************************************************/
#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

int main (int argc, char *argv[])
{
int numtasks, rank, dest, source, rc, count, tag=1;
int inmsg, outmsg=1;
MPI_Status Stat;
int index;
struct 	timeval tv1, tv2;
double totalTime = 0;

MPI_Init(&argc,&argv);
MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
MPI_Comm_rank(MPI_COMM_WORLD, &rank);

if (numtasks < 2) 
{
      printf("ERROR: Must specify two tasks!  Quitting...\n");
      MPI_Abort(MPI_COMM_WORLD, rc);
      exit(0);
}

if (rank == 0)
{
  if (numtasks > 2) 
  {
    printf("Numtasks=%d. Only 2 needed. Ignoring extra tasks...\n",numtasks);
  }

  dest = 1;
  source = 1;
  
  for( index = 0; index < 1000; index++ )
  {
    tv1 = MPI_Wtime();
    rc = MPI_Send(&outmsg, 1, MPI_INT, dest, tag, MPI_COMM_WORLD);
    rc = MPI_Recv(&inmsg, 1, MPI_INT, source, tag, MPI_COMM_WORLD, &Stat);
    tv2 = MPI_Wtime();
    totalTime += tv2 - tv1;
  }
}

else if (rank == 1)
 {
  dest = 0;
  source = 0;

  for( index = 0; index < 1000; index++ )
  {
    rc = MPI_Recv(&inmsg, 1, MPI_INT, source, tag, MPI_COMM_WORLD, &Stat);
    rc = MPI_Send(&outmsg, 1, MPI_INT, dest, tag, MPI_COMM_WORLD);
  }

}

if (rank == 0 ) {
  printf("%d \n", totalTime);
  }

 MPI_Finalize();
}
