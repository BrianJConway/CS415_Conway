#!/bin/bash
#SBATCH -t5
#SBATCH -n4

# running
echo "Parallelized Matrix Multiplication - 4" && srun parallel_matrixmultiplication
