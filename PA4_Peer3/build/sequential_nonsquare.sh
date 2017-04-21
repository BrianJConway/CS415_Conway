#!/bin/bash
#SBATCH -t5
#SBATCH -n1
# running
echo "Sequential Non-Square Matrix Multiplication" && srun sequential_matrixmultiplication_nonsquare
