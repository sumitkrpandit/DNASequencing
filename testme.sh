#!/bin/bash
run_command=$1

eval $run_command mpirun -np 2 testb

