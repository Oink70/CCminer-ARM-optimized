# To change the cuda arch, edit Makefile.am and run ./build.sh
#!/bin/bash

# Get CPU information
arch=$(lscpu | awk '/Architecture:/ { print $2 }')
features=$(lscpu | awk '/Flags:/ { print $0 }')

# Common compiler flags
common_flags="-O3 -ffinite-loops -ffast-math -D_REENTRANT -falign-functions=16 -fomit-frame-pointer -fpic -pthread -flto -fuse-ld=lld -fno-stack-protector"

# Set architecture-specific flags
if [[ "$arch" == "armv8-a" ]]; then
    cpu_flags="-march=armv8-a+crypto -mtune=cortex-a53"
elif [[ "$arch" == "armv7-a" ]]; then
    cpu_flags="-march=armv7-a -mtune=cortex-a9"
elif [[ "$arch" == "armv7ve" ]]; then
    cpu_flags="-march=armv7ve -mtune=cortex-a7"
elif [[ "$arch" == "armv6zk" ]]; then
    cpu_flags="-march=armv6zk -mtune=arm1176jzf-s"
elif [[ "$arch" == "armv6" ]]; then
    cpu_flags="-march=armv6 -mtune=arm1136j-s"
# Add more architectures here...
else
    echo "Unsupported architecture: $arch"
    exit 1
fi

# Set vectorization flags
vectorization_flags="-Rpass-missed=loop-vectorize -Rpass-analysis=loop-vectorize -mllvm -enable-loop-distribute"

# Combine all flags
all_flags="$common_flags $cpu_flags $vectorization_flags"

# Configure and build
./configure CXXFLAGS="-Wl,-hugetlbfs-align -funroll-loops -finline-functions $all_flags" \
            CFLAGS="-Wl,-hugetlbfs-align -finline-functions $all_flags" \
            CXX=clang++ CC=clang LDFLAGS="-v -flto -Wl,-hugetlbfs-align"
