ROCM_SUPPORT=OFF
LZ_SUPPORT=OFF
CUDA_SUPPORT=OFF
ENERGY_PROFILING=ON

compiler_path=""
platform=""
arch=""

mkdir -p build
cd build

while [[ $# -gt 0 ]]; do
  case "$1" in
    --compiler=*)
      compiler_path="${1#*=}"
      shift
      ;;
    --plat=*)
      plat="${1#*=}"
      shift
      ;;
    --arch=*)
      arch="${1#*=}"
      shift
      ;;
    *)
    echo "Invalid argument: $1"
      return 1 2>/dev/null
      exit 1
      ;;
  esac
done

if [ -z "$plat" ]; then
  echo "Platform not specified, use --plat=<rocm|lz|cuda>"
  exit 1
fi

if [ -z "$arch" ]; then
  echo "Architecture not specified, use --arch=<gpu_arch>"
  exit 1
fi

if [ -z "$compiler_path" ]; then
  echo "Compiler path not specified, use --compiler=<path_to_compiler>"
  exit 1
fi

if [ "$plat" == "rocm" ]; then
  ROCM_SUPPORT=ON
  ENERGY_PROFILING=OFF
elif [ "$plat" == "lz" ]; then
  LZ_SUPPORT=ON
elif [ "$plat" == "cuda" ]; then
  CUDA_SUPPORT=ON
else
  echo "Invalid platform: $plat"
  echo "Valid platforms: rocm, lz, cuda"
  exit 1
fi

cmake \
  -DCMAKE_CXX_COMPILER="$compiler_path" \
  -DSYNERGY_SYCL_IMPL=DPC++ \
  -DARCH="$arch" \
  -DENABLED_SYNERGY=ON \
  -DSYNERGY_ROCM_SUPPORT=$ROCM_SUPPORT \
  -DDPCPP_WITH_ROCM_BACKEND=$ROCM_SUPPORT \
  -DSYNERGY_LZ_SUPPORT=$LZ_SUPPORT \
  -DDPCPP_WITH_LZ_BACKEND=$LZ_SUPPORT \
  -DSYNERGY_CUDA_SUPPORT=$CUDA_SUPPORT \
  -DDPCPP_WITH_CUDA_BACKEND=$CUDA_SUPPORT \
  -DSYNERGY_USE_PROFILING_ENERGY=$ENERGY_PROFILING \
  -DSYNERGY_DEVICE_PROFILING=ON \
  -DSYNERGY_KERNEL_PROFILING=ON \
  -DSYNERGY_HOST_PROFILING=ON \
  ..

cmake --build .