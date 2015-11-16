#!/bin/bash

#specify export QSUB vars to shell script
#$ -V -R y -q parallel.q -N run_mpi.sh

#execute script from current working directory 
#$ -cwd

#select parallel environment to run on 1 node
#$ -pe openmpi 6

#select max run-time
#$ -l h_rt=00:01:00

#number of cores (up to 6x number of nodes)
NUM_CORES=6

#name of parallel executable
#note: binary stored locally on each node
EXEC=$PWD/hello



#run castep
mpirun -np $NUM_CORES $EXEC $1

echo ""
echo "Finished: "`date`
echo ""

