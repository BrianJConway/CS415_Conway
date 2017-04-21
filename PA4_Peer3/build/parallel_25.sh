#!/bin/bash
#SBATCH -t5
#SBATCH -n25

# running
echo "Parallelized Matrix Multiplication - 25" && srun parallel_matrixmultiplication
