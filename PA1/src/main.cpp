#include <mpi.h>
#include <cstdlib>
#include <time.h>
#include <iostream>
#include "Timer.h"
#include <iomanip>

using namespace std;
const double NUM_MESSAGES = 10000;

int main(int argc, char *argv[])
{
    int numtasks, rank, dest, source, rc, tag = 1;
    int inmsg, outmsg = 1;
    MPI_Status Stat;
    int index;
    Timer timer;
    double totalTime = 0;
    bool started = false;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (numtasks < 2)
    {
        cout << "Must specify at least two tasks. Terminating..." << endl;
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

        for (index = 0; index < NUM_MESSAGES; index++)
        {
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
        }
    }

    else if (rank == 1)
    {
        dest = 0;
        source = 0;

        for (index = 0; index < NUM_MESSAGES; index++)
        {   
            rc = MPI_Recv(&inmsg, 1, MPI_INT, source, tag, MPI_COMM_WORLD, &Stat);
            rc = MPI_Send(&outmsg, 1, MPI_INT, dest, tag, MPI_COMM_WORLD);
        }
    }

    if (rank == 0)
    {
        totalTime = timer.getElapsedTime() / NUM_MESSAGES;
        cout << "Time: " << setprecision(6) << totalTime << endl;
    }

    MPI_Finalize();
}
