# PA4: "Matrix Multiplication"

## Building and Running

This project includes a makefile which will generate object files and an executable file for sequential matrix multiplication. 

There are a number of scripts depending on the dimensions of the matrices.

For sequential matrix multiplication, scripts are run like so:

sbatch /scripts/seq_[NUMBER].sh

NUMBER represents the dimensions of the matrix. It must be one of the following
numbers: 100, 200, 500, 1000, 1250, 1500, 1750, 2000, 2250, 2500, 2650.

If you want to view the A, B, and C matrices, add the letter "y" as a command line parameter
to the end of any of the script files themselves. Matrices will be output
to the file "output.txt" after the script is run.

### Makefile Instructions

```bash
cd build
make
sbatch /scripts/seq_[NUMBER].sh
make clean
```

For example:

Sequential, 1000x1000 matrices
```bash
sbatch /scripts/seq_1000.sh
```

