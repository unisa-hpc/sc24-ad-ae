# first arg frequency scaling approach APP | KERNEL | PHASE
APPROACH=$1
# second arg MPI frequency change hiding 1 or 0
HIDING=$2
# paht to the cxx oneApi compiler
CXX_COMPILER=$3
cmake \
  -DCMAKE_CXX_COMPILER="$CXX_COMPILER" \
  -DCMAKE_CXX_FLAGS=" -lmpi -Wno-deprecated -fsycl -fsycl-targets=nvptx64-nvidia-cuda -Xsycl-target-backend --cuda-gpu-arch=sm_80 -O3" \
  -DSYCL_RUNTIME=DPCPP \
  -DSYNERGY_SYCL_IMPL="DPC++" \
  -DSYNERGY_KERNEL_PROFILING=OFF\
  -DSYNERGY_DEVICE_PROFILING=ON \
  -DSYNERGY_HOST_PROFILING=OFF \
  -DSYNERGY_USE_PROFILING_ENERGY=ON \
  -DSYNERGY_CUDA_SUPPORT=ON \
  -DMPI_AS_LIBRARY=OFF \
  -DSYNERGY_BUILD_SAMPLES=ON \
  -DSYNERGY_SAMPLES_CUDA_ARCH="sm_80" \
  -DCLOVERLEAF_PRINT=OFF \
  -DFREQ_SCALING_APPROACH="$APPROACH" \
  -DMPI_HIDING=$HIDING \
  -DHARDWARE_VENDOR="NVIDIA" \
  ..