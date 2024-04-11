# num of runs
NUM_GPUS=$1
NUM_NODES=$2
NUM_RUNS=$3
# create the path to build directory
SCRIPT_DIR=$(dirname "$(readlink -f "$0")")
BUILD_DIR=$SCRIPT_DIR/../build
LOG_DIR=logs_all_${NUM_GPUS}gpus_${NUM_NODES}nodes_${NUM_RUNS}runs
if [ ! -d "$SCRIPT_DIR/../${LOG_DIR}" ]; then
    # Create the directory
    mkdir -p "$SCRIPT_DIR/../${LOG_DIR}"
    echo "Directory created: $SCRIPT_DIR/../${LOG_DIR}"
else
    echo "Directory already exists: $SCRIPT_DIR/../${LOG_DIR}"
    rm -rf $SCRIPT_DIR/../${LOG_DIR}/*
fi


## APP
rm -rf $BUILD_DIR/*
cd $BUILD_DIR
${SCRIPT_DIR}/compile_miniweather.sh APP 0
make -j 
#TODO: change with the per_app frequency
mem_freq=0
core_freq=0
for ((i=0; i<$NUM_RUNS;i++));
do
    echo Run $i 
    mpirun -n ${NUM_GPUS} ./parallelfor  >> ../${LOG_DIR}/miniweather_per_app.log
    nvidia-smi -ac $memfreq,$corefreq 
done

## KERNEL
rm -rf $BUILD_DIR/*
cd $BUILD_DIR
${SCRIPT_DIR}/compile_miniweather.sh KERNEL 0
make -j 
for ((i=0; i<$NUM_RUNS;i++));
do
    echo Run $i 
    mpirun -n ${NUM_GPUS} ./parallelfor  >> ../${LOG_DIR}/miniweather_per_kernel.log
    nvidia-smi -ac
    nvidia-smi -gc
done


## NO_HIDING
rm -rf $BUILD_DIR/*
cd $BUILD_DIR
${SCRIPT_DIR}/compile_miniweather.sh PHASE 0
make -j 
for ((i=0; i<$NUM_RUNS;i++));
do
    echo Run $i 
    mpirun -n ${NUM_GPUS} ./parallelfor  >> ../${LOG_DIR}/miniweather_no_hiding.log
    nvidia-smi -ac
    nvidia-smi -gc
done

## HIDING
rm -rf $BUILD_DIR/*
cd $BUILD_DIR
${SCRIPT_DIR}/compile_miniweather.sh PHASE 1
make -j 
for ((i=0; i<$NUM_RUNS;i++));
do
    echo Run $i 
    mpirun -n ${NUM_GPUS} ./parallelfor  >> ../${LOG_DIR}/miniweather_hiding.log
    nvidia-smi -gc
    nvidia-smi -ac
done




