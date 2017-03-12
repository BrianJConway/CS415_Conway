#!/bin/bash
#SBATCH -n 20
#SBATCH -N 1
#SBATCH --mem=2048MB
#SBATCH --time=00:10:00
#SBATCH --mail-user=YOUR_EMAIL@DOMAIN.COM
#SBATCH --mail-type=ALL

srun mpi_static
