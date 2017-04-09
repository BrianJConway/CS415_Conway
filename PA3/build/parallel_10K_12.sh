#!/bin/bash
#SBATCH -n 12
#SBATCH --mem=2048MB
#SBATCH --time=00:05:30
#SBATCH --mail-user=YOUR_EMAIL@DOMAIN.COM
#SBATCH --mail-type=ALL

srun mpi_parallel 10000 y
