#include <mpi.h>
#include <cstdlib>
#include <time.h>
#include <iostream>
#include "Timer.h"

using namespace std;

int main(int argc, char *argv[])
{
    int numtasks, rank, dest, source, rc, tag = 1;
    int inmsg, outmsg = 1;
    MPI_Status Stat;
    int index;
    Timer timer;
    double t1, t2, totalTime = 0;
    char time[11];
    bool started = false;

    MPI_Init(&argc, &argv);
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
            printf("Numtasks=%d. Only 2 needed. Ignoring extra tasks...\n", numtasks);
        }

        dest = 1;
        source = 1;

        for (index = 0; index < 10000; index++)
        {
            t1 = MPI_Wtime();
            if( !started )
            {
                started = true;
                timer.start();
            }
            else
            {
                timer.resume();
            }

            rc = MPI_Send(&outmsg, 1, MPI_INT, dest, tag, MPI_COMM_WORLD);
            rc = MPI_Recv(&inmsg, 1, MPI_INT, source, tag, MPI_COMM_WORLD, &Stat);
            timer.stop();
            t2 = MPI_Wtime();
            totalTime += t2 - t1;
        }
            timer.getElapsedTime(time);
    }

    else if (rank == 1)
    {
        dest = 0;
        source = 0;

        for (index = 0; index < 1000; index++)
        {   
            rc = MPI_Recv(&inmsg, 1, MPI_INT, source, tag, MPI_COMM_WORLD, &Stat);
            rc = MPI_Send(&outmsg, 1, MPI_INT, dest, tag, MPI_COMM_WORLD);
        }
    }

    if (rank == 0)
    {
        cout << "WTime: " << totalTime << endl
             << "Timeval: " << time << endl;

    }

    MPI_Finalize();
}
