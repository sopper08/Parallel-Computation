#!/bin/bash 
#PBS -P ACD107076
#PBS -N p4 
#PBS -q ctest 
#PBS -l select=2:ncpus=4:mpiprocs=4
#PBS -j oe
module purge 
module load intel/2018_u1 
module list 
cd $PBS_O_WORKDIR

mpirun ../hw4-12_p
