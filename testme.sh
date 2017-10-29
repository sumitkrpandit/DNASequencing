#!/bin/bash
run_command=$1

eval $run_command mpirun -np 4 DNASequencingBoost < in.txt
if test $? == 0
then
  printf "\033[0;32m[PASSED]\033[0m\n"
else
  printf "\033[1;31mError:: while running DNASequencingBoost.\033[0m\n"
fi
