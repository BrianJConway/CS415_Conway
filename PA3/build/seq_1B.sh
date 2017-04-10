#!/bin/bash
#SBATCH -n 1
#SBATCH --mem=2048MB
#SBATCH --time=00:06:30
#SBATCH --mail-user=YOUR_EMAIL@DOMAIN.COM
#SBATCH --mail-type=ALL

srun mpi_sequential 1000000000
