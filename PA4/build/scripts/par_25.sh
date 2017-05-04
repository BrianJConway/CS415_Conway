#!/bin/bash
#SBATCH -n 25
#SBATCH -N 4
#SBATCH --cpus-per-task=1
#SBATCH --mem=8192MB
#SBATCH --time=00:05:00
#SBATCH --mail-type=ALL

srun mpi_parallel_file A.txt B.txt y