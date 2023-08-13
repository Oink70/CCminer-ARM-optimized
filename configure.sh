#!/bin/bash

# Get CPU information
arch=$(lscpu | awk '/Architecture:/ { print $2 }')
model_name=$(lscpu | awk -F ': +' '/Model name:/ { print $2 }')

# Common compiler flags
common_flags="-O3 -ffinite-loops -ffast-math -D_REENTRANT -falign-functions=16 -fomit-frame-pointer -fpic -pthread -flto -fuse-ld=lld -fno-stack-protector"

# Set architecture-specific flags
if [[ "$arch" == "aarch64" ]]; then
    case "$model_name" in
        "Cortex-A72")
            cpu_flags="-march=armv8-a+crc+crypto -mtune=cortex-a72"
            ;;
        "Cortex-A73")
            cpu_flags="-march=armv8-a+crc+crypto -mtune=cortex-a73"
            ;;
        "Cortex-A75")
            cpu_flags="-march=armv8.2-a+fp16+fp16fml+fp16fmla+crc+crypto -mtune=cortex-a75"
            ;;
        "Cortex-A53")
            cpu_flags="-march=armv8-a+crypto -mtune=cortex-a53"
            ;;
        *)
            # Default to ARMv8-A architecture (Cortex-A53) if unknown
            echo "Unknown or unsupported model: $model_name. Defaulting to ARMv8-A."
            cpu_flags="-march=armv8-a+crypto -mtune=cortex-a53"
            ;;
    esac
else
    # Default to ARMv8-A architecture (Cortex-A53) if unknown
    echo "Unknown or unsupported architecture: $arch. Defaulting to ARMv8-A."
    cpu_flags="-march=armv8-a+crypto -mtune=cortex-a53"
fi

# Mitigation flags
mitigation_flags="-mindirect-branch=thunk -mindirect-branch-register"

# Set vectorization flags
vectorization_flags="-Rpass-missed=loop-vectorize -Rpass-analysis=loop-vectorize -mllvm -enable-loop-distribute"

# Combine all flags
all_flags="$common_flags $cpu_flags $mitigation_flags $vectorization_flags"

# Configure and build
./configure CXXFLAGS="-Wl,-hugetlbfs-align -funroll-loops -finline-functions $all_flags" \
            CFLAGS="-Wl,-hugetlbfs-align -finline-functions $all_flags" \
            CXX=clang++ CC=clang LDFLAGS="-v -flto -Wl,-hugetlbfs-align"
