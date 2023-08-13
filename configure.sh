#!/bin/bash

# Get CPU information
arch=$(lscpu | awk '/Architecture:/ { print $2 }')
features=$(lscpu | awk '/Flags:/ { print $0 }')

# Common compiler flags
common_flags="-O3 -ffinite-loops -ffast-math -D_REENTRANT -falign-functions=16 -fomit-frame-pointer -fpic -pthread -flto -fuse-ld=lld -fno-stack-protector"

# Set architecture-specific flags
if [[ "$arch" == "armv8-a" ]]; then
    case "$features" in
        *"cortex-a72"*)
            cpu_flags="-march=armv8-a+crc+crypto -mtune=cortex-a72"
            ;;
        *"cortex-a73"*)
            cpu_flags="-march=armv8-a+crc+crypto -mtune=cortex-a73"
            ;;
        *"cortex-a75"*)
            cpu_flags="-march=armv8.2-a+fp16+fp16fml+fp16fmla+crc+crypto -mtune=cortex-a75"
            ;;
        *)
            # Default to ARMv8-A architecture (Cortex-A53) if unknown
            echo "Unknown or unsupported architecture: $arch. Defaulting to ARMv8-A."
            cpu_flags="-march=armv8-a+crypto -mtune=cortex-a53"
            ;;
    esac
else
    # Default to ARMv8-A architecture (Cortex-A53) if unknown
    echo "Unknown or unsupported architecture: $arch. Defaulting to ARMv8-A."
    cpu_flags="-march=armv8-a+crypto -mtune=cortex-a53"
fi

# Set vectorization flags
vectorization_flags="-Rpass-missed=loop-vectorize -Rpass-analysis=loop-vectorize -mllvm -enable-loop-distribute"

# Combine all flags
all_flags="$common_flags $cpu_flags $vectorization_flags"

# Configure and build
./configure CXXFLAGS="-Wl,-hugetlbfs-align -funroll-loops -finline-functions $all_flags" \
            CFLAGS="-Wl,-hugetlbfs-align -finline-functions $all_flags" \
            CXX=clang++ CC=clang LDFLAGS="-v -flto -Wl,-hugetlbfs-align"
