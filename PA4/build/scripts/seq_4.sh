#!/bin/bash
#SBATCH -n 1
#SBATCH -N 1
#SBATCH --cpus-per-task=1
#SBATCH --mem=8192MB
#SBATCH --time=00:05:00
#SBATCH --mail-type=ALL

srun mpi_sequential 4 y
