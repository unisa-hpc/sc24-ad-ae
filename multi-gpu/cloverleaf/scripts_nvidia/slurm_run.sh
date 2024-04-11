#!/bin/bash
##TODO:complete this info 
#SBATCH --account=
#SBATCH --partition=
#SBATCH --gpus-per-node=
#SBATCH --ntasks-per-node=
#SBATCH --mail-user=
#SBATCH --job-name=cloverleaf_ws_test
#SBATCH --time=00:05:00
#SBATCH --gres=nvgpufreq
#SBATCH --mail-type=ALL
#SBATCH --exclusive

num_gpus=$1
nodes=$2
executable=$3
path=$4
num_runs=$5
# echo ${num_gpus}
# echo ${nodes}
# echo ${executable}
# echo ${path}

size=$(( $num_gpus*32 )) #TODO: change this 32 depending on the max size on a single GPU
#TODO: -gpu parameter is needed??
mpirun -n $num_gpus -gpu ./${executable} --file $path/cloverleaf_sycl/input_file/clover_bm${size}_short.in 2>> $path/cloverleaf_sycl/logs_${num_gpus}gpus_${nodes}nodes_${num_runs}runs/${executable} 