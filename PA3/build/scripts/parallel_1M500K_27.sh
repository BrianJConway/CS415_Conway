#!/bin/bash
#SBATCH -n 27
#SBATCH -N 4
#SBATCH --cpus-per-task=1
#SBATCH --mem=8192MB
#SBATCH --time=00:05:30
#SBATCH --mail-user=brianc@nevada.unr.edu
#SBATCH --mail-type=ALL

srun mpi_parallel 1500000
