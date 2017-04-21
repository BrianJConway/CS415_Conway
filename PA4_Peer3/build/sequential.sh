#!/bin/bash
#SBATCH -t5
#SBATCH -n1
# running
echo "Sequential Matrix Multiplication" && srun sequential_matrixmultiplication
