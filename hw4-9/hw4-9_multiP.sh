#!/bin/bash 
#PBS -P ACD107076
#PBS -N hw4-9_multiP 
#PBS -q ctest 
#PBS -l select=1:ncpus=1:mpiprocs=1 
#PBS -l place=scatter 
#PBS -l walltime=00:01:00 
#PBS -j n 
module purge 
module load intel/2018_u1 
module list 
cd $PBS_O_WORKDIR

mpirun ./hw4-9_multiP.o
