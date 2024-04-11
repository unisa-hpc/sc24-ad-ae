#!/bin/bash

# Create an array with the specified numbers
NUM_GPUS=(2 4 8 16)
NUM_NODES=(1 1 2 4)
NUM_RUNS=1
for ((i=0; i<${#numbers[@]}; i++)); do
    ./run.sh "${NUM_GPUS[i]}" "${NUM_NODES[i]}" ${NUM_RUNS}
done