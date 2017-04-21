#!/bin/bash

#this script is run from PA*/build#path to batch files: PA*/build/batchf/para/
#<username> means whatever name you use to login to h1

BATCHFILES=./parallel_*.sh

for batchfile in $BATCHFILES
do
    TEST=$(squeue -o"%.20u")
    while [[ "$TEST" =~ "cpollock" ]]
    do
      sleep 1s
      TEST=$(squeue -o"%.20u")
    done
    sbatch $batchfile
    sleep 1s
done
