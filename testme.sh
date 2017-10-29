#!/bin/bash
run_command=$1

RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\003[0m'

eval $run_command mpirun -np 4 DNASequencingBoost < in.txt
if test $? != 0
then
  echo -e "Error:: while running DNASequencingBoost."
else
  echo -e "[PASSED]"
fi
