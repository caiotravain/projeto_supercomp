#!/bin/bash
#SBATCH --job-name=mpi
#SBATCH --partition=espec
#SBATCH --nodes=2
#SBATCH --ntasks-per-node=5  # 5 task por maquina
#SBATCH --cpus-per-task=1    # Cada task usa uma CPU
#SBATCH --mem=1G
#SBATCH --output=mpi.out
time mpirun ./3mpi < grafo.txt