cmake \
-DCMAKE_CXX_COMPILER="/leonardo/home/userexternal/lcarpent/intel/oneapi/compiler/2024.0/bin/icpx" \
-DCMAKE_CXX_FLAGS="-lmpi -ffast-math -Wno-deprecated -fsycl -fsycl-targets=nvptx64-nvidia-cuda -Xsycl-target-backend=nvptx64-nvidia-cuda --cuda-gpu-arch=sm_80 -O3" \
-DCUDAToolkit_NVCC_EXECUTABLE="/leonardo/prod/opt/compilers/cuda/11.8/none/bin/nvcc" \
-DSYNERGY_SYCL_IMPL="DPC++" \
-DSYNERGY_KERNEL_PROFILING="OFF" \
-DSYNERGY_DEVICE_PROFILING="ON" \
-DSYNERGY_HOST_PROFILING="OFF" \
-DSYNERGY_CUDA_SUPPORT="ON" \
-DYAKL_ARCH=SYCL \
-DYAKL_SYCL_FLAGS="-fsycl -fsycl-targets=nvptx64-nvidia-cuda -O3" \
-DNX=1024 \
-DNZ=1024 \
-DLDFLAGS="-lpnetcdf" \
..
# -DMPI_AS_LIBRARY="ON" \
# -DMPI_C_LIB_DIR="/leonardo/prod/spack/03/install/0.19/linux-rhel8-icelake/gcc-11.3.0/openmpi-4.1.4-si334jjzldoxyl3jbbb3cf6legzshlqn/lib/" \
# -DMPI_C_INCLUDE_DIR="/leonardo/prod/spack/03/install/0.19/linux-rhel8-icelake/gcc-11.3.0/openmpi-4.1.4-si334jjzldoxyl3jbbb3cf6legzshlqn/include/" \
# -DMPI_C_LIB="/leonardo/prod/spack/03/install/0.19/linux-rhel8-icelake/gcc-11.3.0/openmpi-4.1.4-si334jjzldoxyl3jbbb3cf6legzshlqn/lib/libmpi.so" \

