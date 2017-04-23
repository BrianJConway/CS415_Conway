#!/bin/bash
#SBATCH --job-name="MatrixMulplication-Parallel"
#SBATCH --output="MatrixMulplication-Parallel"
#SBATCH -n 9
#SBATCH --time=00:06:00

srun Parallel 9
