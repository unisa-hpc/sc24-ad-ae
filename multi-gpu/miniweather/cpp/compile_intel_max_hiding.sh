cmake \
  -DCMAKE_CXX_COMPILER="/opt/intel/oneapi/compiler/2024.0/bin/icpx" \
  -DCMAKE_CXX_FLAGS="-lmpi -ffast-math -Wno-deprecated -fsycl -fsycl-targets=intel_gpu_pvc -O3" \
  -DSYNERGY_SYCL_IMPL="DPC++" \
  -DSYNERGY_KERNEL_PROFILING=OFF \
  -DSYNERGY_DEVICE_PROFILING=ON \
  -DSYNERGY_HOST_PROFILING=OFF \
  -DSYNERGY_USE_PROFILING_ENERGY=ON \
  -DSYNERGY_LZ_SUPPORT=ON \
  -DYAKL_ARCH=SYCL \
  -DYAKL_SYCL_FLAGS="-fsycl -fsycl-targets=intel_gpu_pvc -O3" \
  -DNX=128 \
  -DNZ=64 \
  -DMPI_HIDING=1 \
  -DSIM_TIME=10 \
  -DLDFLAGS="-lpnetcdf" \
  ..