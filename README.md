# ccminer for ARM (cortex-a53)

Based on https://github.com/monkins1010/ccminer/tree/ARM

Git and Build Process:
```
sudo apt-get update
sudo apt-get install libcurl4-openssl-dev libssl-dev libjansson-dev automake autotools-dev build-essential -y
sudo apt-get install -y libllvm-16-ocaml-dev libllvm16 llvm-16 llvm-16-dev llvm-16-doc llvm-16-examples llvm-16-runtime clang-16 clang-tools-16 clang-16-doc libclang-common-16-dev libclang-16-dev libclang1-16 clang-format-16 python3-clang-16 clangd-16 clang-tidy-16 libclang-rt-16-dev libpolly-16-dev libfuzzer-16-dev lldb-16 lld-16 libc++-16-dev libc++abi-16-dev libomp-16-dev libclc-16-dev libunwind-16-dev libmlir-16-dev mlir-16-tools flang-16 libclang-rt-16-dev-wasm32 libclang-rt-16-dev-wasm64 libclang-rt-16-dev-wasm32 libclang-rt-16-dev-wasm64
git clone https://github.com/Oink70/CCminer-ARM-optimized.git
cd CCminer-ARM-optimized
chmod +x build.sh
chmod +x configure.sh
chmod +x autogen.sh
CXX=clang++ CC=clang build.sh
```

For specific details on installing clang-16 on your current OS, check: https://apt.llvm.org/
