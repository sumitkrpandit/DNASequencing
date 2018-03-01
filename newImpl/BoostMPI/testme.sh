#!/bin/bash
run_command=$1

eval $run_command mpirun -np 2 CacheDNASequencingBoost < ../common/inputFiles/10.dat > out.txt
if test $? == 0
then
  printf "\033[0;32m[PASSED]\033[0m\n"
else
  printf "\033[1;31mError:: while running CacheDNASequencingBoost.\033[0m\n"
fi
