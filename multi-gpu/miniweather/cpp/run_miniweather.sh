# num of runs
NUM_RUNS=50
# create the path to build directory
SCRIPT_DIR=$(dirname "$(readlink -f "$0")")
BUILD_DIR=$SCRIPT_DIR/build

if [ ! -d "$SCRIPT_DIR/logs" ]; then
    # Create the directory
    mkdir -p "$SCRIPT_DIR/logs"
    echo "Directory created: $SCRIPT_DIR/logs"
else
    echo "Directory already exists: $SCRIPT_DIR/logs"
    rm -rf $SCRIPT_DIR/logs/*
fi

# asynch + no_hiding
rm -rf $BUILD_DIR/*
../compile_intel_max_no_hiding.sh
make -j 
mpirun -n 4 ./parallelfor  
for ((i=0; i<$NUM_RUNS;i++));
do
    echo Run $i 
    mpirun -n 4 ./parallelfor  >> ../logs/miniweather_no_hiding.log
    intel_gpu_frequency -d
done

# # asynch + hiding
# # clean the build directory
rm -rf $BUILD_DIR/*
cd $BUILD_DIR
../compile_intel_max_hiding.sh 
make -j 

mpirun -n 4 ./parallelfor  
for ((i=0; i<$NUM_RUNS;i++));
do
    echo Run $i 
    mpirun -n 4 ./parallelfor  >> ../logs/miniweather_hiding.log
    intel_gpu_frequency -d
done
