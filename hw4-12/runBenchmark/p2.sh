#!/bin/bash 
#PBS -P ACD107076
#PBS -N p2
#PBS -q ctest 
#PBS -l select=2:ncpus=2:mpiprocs=2
#PBS -j oe
module purge 
module load intel/2018_u1 
module list 
cd $PBS_O_WORKDIR

mpirun ../hw4-12_p
