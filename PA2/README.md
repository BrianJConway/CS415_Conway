# PA1: "Ping Pong"

# Building and Running

## Building and Running
This project includes a makefile which will generate object files and two executables, one for sending a single integer between two nodes and one for sending 
bigger and bigger arrays of integers between two nodes. The script One_box.sh will run the mpi_pingPong exectuable for two nodes within the same machine. 
The script Two_box.sh will run the mpi_pingPong executable for two nodes in different machines. The script Timing.sh will run the mpi_bufferChecker 
executable for two nodes in different machines. Each exectuable, when run, will output results as a file called "measurements.csv.".

### Makefile Instructions

```bash
make
sbatch One_box.sh
sbatch Two_Box.sh
sbatch Timing.sh
make clean
```
