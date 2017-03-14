#!/bin/bash
#SBATCH -n 3
#SBATCH --mem=2048MB
#SBATCH --time=00:05:00
#SBATCH --mail-user=YOUR_EMAIL@DOMAIN.COM
#SBATCH --mail-type=ALL

srun mpi_static
