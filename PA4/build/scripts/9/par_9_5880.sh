#!/bin/bash
#SBATCH -n 9
#SBATCH -N 2
#SBATCH --cpus-per-task=1
#SBATCH --mem=8192MB
#SBATCH --time=00:10:00
#SBATCH --mail-type=ALL

srun mpi_parallel 5880