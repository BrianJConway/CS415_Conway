#!/bin/bash
#SBATCH -t5
#SBATCH -n9

# running
echo "Parallelized Matrix Multiplication - 9" && srun parallel_matrixmultiplication
