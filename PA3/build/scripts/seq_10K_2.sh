#!/bin/bash
#SBATCH -n 1
#SBATCH -N 1
#SBATCH --cpus-per-task=1
#SBATCH --mem=8192MB
#SBATCH --time=00:05:30
#SBATCH --mail-user=brianc@nevada.unr.edu
#SBATCH --mail-type=ALL

srun mpi_sequential 10000 y 2
