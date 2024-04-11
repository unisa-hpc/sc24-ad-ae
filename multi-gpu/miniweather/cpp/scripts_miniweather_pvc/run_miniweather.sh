# num of runs
NUM_RUNS=60
# create the path to build directory
SCRIPT_DIR=$(dirname "$(readlink -f "$0")")
BUILD_DIR=$SCRIPT_DIR/../build
LOG_DIR=logs_all_60runs
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

for ((i=0; i<$NUM_RUNS;i++));
do
    echo Run $i 
    mpirun -n 4 ./parallelfor  >> ../${LOG_DIR}/miniweather_per_app.log
    intel_gpu_frequency -d
done

## KERNEL
rm -rf $BUILD_DIR/*
cd $BUILD_DIR
${SCRIPT_DIR}/compile_miniweather.sh KERNEL 0
make -j 
for ((i=0; i<$NUM_RUNS;i++));
do
    echo Run $i 
    mpirun -n 4 ./parallelfor  >> ../${LOG_DIR}/miniweather_per_kernel.log
    intel_gpu_frequency -d
done


## NO_HIDING
rm -rf $BUILD_DIR/*
cd $BUILD_DIR
${SCRIPT_DIR}/compile_miniweather.sh PHASE 0
make -j 
for ((i=0; i<$NUM_RUNS;i++));
do
    echo Run $i 
    mpirun -n 4 ./parallelfor  >> ../${LOG_DIR}/miniweather_no_hiding.log
    intel_gpu_frequency -d
done

## HIDING
rm -rf $BUILD_DIR/*
cd $BUILD_DIR
${SCRIPT_DIR}/compile_miniweather.sh PHASE 1
make -j 
for ((i=0; i<$NUM_RUNS;i++));
do
    echo Run $i 
    mpirun -n 4 ./parallelfor  >> ../${LOG_DIR}/miniweather_hiding.log
    intel_gpu_frequency -d
done




