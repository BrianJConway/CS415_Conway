#!/bin/bash
#SBATCH -t5
#SBATCH -n16

# running
echo "Parallelized Matrix Multiplication - 16" && srun parallel_matrixmultiplication
