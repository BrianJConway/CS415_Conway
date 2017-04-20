#!/bin/bash
#SBATCH -n 9
#SBATCH -N 2
#SBATCH --cpus-per-task=1
#SBATCH --mem=8192MB
#SBATCH --time=00:05:00
#SBATCH --mail-user=brianc@nevada.unr.edu
#SBATCH --mail-type=ALL

srun mpi_parallel 20 y
