# PA4: "Matrix Multiplication"

## Building and Running

This project includes a makefile which will generate object files and executables for sequential and parallel matrix multiplication. 

NOTE: The parallel file intended for submission is "parallel_file.cpp", which
meets the requirements of loading the matrices from their individual files and 
outputting the resulting matrix to a file. The file "parallel_generated.cpp"
is included if you want to just have the program generate numbers for matrices
A and B, then each process outputs the resulting chunks to the console. 

Sequential files were included for easy comparison with parallel results. Both 
the sequential scripts and the parallel scripts that do number generation
will generate the same matrices A and B, and get the same results for C.

There are a number of scripts depending on the dimensions of the matrices,
and for parallel the number of processes as well as the dimensions of the 
matrices.

### Sequential 

For sequential matrix multiplication, scripts are run like so:

sbatch /scripts/seq_[NUMBER].sh

NUMBER represents the dimensions of the matrix. It must be one of the following
numbers: 4, 6, 8, 10, 20, 100, 200

For sequential, matrices of size 20 and under will be output to the file "seqOutput.txt" after the script is run.
If you want to view the A, B, and C matrices for larger sizes, add the letter "y" as a command line parameter
to the end of any of the script files themselves. 

HIGHLY RECCOMMENDED: If you would like to run with custom sizes, feel free
to edit the script files. The command line parameters are like so:

mpi_sequential [Matrix_Size] [OUTPUT_FLAG]

Matrix_Size: dimensions of matrices. 5 will specify 5x5 matrices.
OUTPUT_FLAG: optional, specifies whether to output results to file. 
Pass "y" here if you want to see the results.

### Parallel


#### Parallel with input files
For parallel matrix multiplication with input files, scripts are run like so:

sbatch /scripts/par_[PROCESSES].sh

PROCESSES represents the number of processors. It must be one of the following
numbers: 4, 9, 16, 25. 

The scripts assume matrix A is in the file A.txt, and matrix B is in the file
B.txt. The program will not check if your matrix sizes are compatible with the 
number of processes specified.

For parallel, timing results will be output to the console. The resulting matrix
C will be output to the file "C.txt".

If you would like to run with custom file names, feel free to edit the script
files. The command line parameters are like so:

mpi_parallel_file [Matrix A file] [Matrix B file] [OUTPUT_FLAG]

OUTPUT_FLAG: optional, specifies whether to output results to "C.txt". 
Pass "y" here if you want to see the results.

#### Parallel with generated input
For parallel matrix multiplication, scripts are run like so:

sbatch /scripts/par_[PROCESSES]_[NUMBER].sh

PROCESSES represents the number of processors. It must be one of the following
numbers: 4, 9, 16, 25. 

NUMBER represents the dimensions of the matrix. It must be one of the following
numbers: 4, 6, 8, 10, 20, 100, 200. 

NOTE: There are no scripts for matrix sizes less than the processor count.

For parallel, all results will be output to the console. I reccommend comparing 
with sequential if you'd like to confirm the parallel is working correctly.

HIGHLY RECCOMMENDED: If you would like to run with custom sizes and amounts of
processes, feel free to edit the script files. The command line parameters are like so:

mpi_parallel [Matrix_Size] [OUTPUT_FLAG]

Matrix_Size: dimensions of matrices. 5 will specify 5x5 matrices.
OUTPUT_FLAG: optional, specifies whether to output results to console. 
Pass "y" here if you want to see the results.

The first thing the console will output is number of tasks and matrix size
adjusted to if you entered not a perfect square for the process count and
an incompatible matrix size. When comparing with sequential, be sure to compare
to the adjusted matrix size.

NOTE: If customizing the scripts, be sure to set the right -N number for your
corresponding -n number.


### Makefile Instructions

```bash
cd build
make
sbatch /scripts/seq_[NUMBER].sh
sbatch /scripts/par_[PROCESSES].sh              // with input files
sbatch /scripts/par_[PROCESSES]_[NUMBER].sh     // with generated input
make clean
```

For example:

Sequential, 4x4 matrices
```bash
sbatch /scripts/seq_4.sh
```
Output will be in "seqOutput.txt"

Parallel, 4 processes, matrices in input files
```bash
sbatch /scripts/par_4.sh
```
Output will be in console and in "C.txt".


Parallel, 4 processes, 4x4 matrices
```bash
sbatch /scripts/par_4_4.sh
```
Output will be in console.

