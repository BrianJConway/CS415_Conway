#!/bin/bash
#SBATCH -N 2
#SBATCH --mem=2048MB
#SBATCH --time=00:30:00
#SBATCH --mail-user=YOUR_EMAIL@DOMAIN.COM
#SBATCH --mail-type=ALL

srun mpi_pingPong
