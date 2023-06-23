# VMPilot: A Modern C++ Virtual Machine SDK

VMPilot is an advanced virtual machine software development kit (SDK) implemented in C++. **Secure by design**, VMPilot is specifically engineered to safeguard your software from reverse engineering. Offering seamless integration and ease of use for your projects, VMPilot sets a new standard for software protection.

Unlike traditional black box solutions, VMPilot is built with transparency in mind. Its inner workings are easily understandable, yet formidable to crack. By incorporating modern cryptography and obfuscation techniques, your software is shielded against potential attacks. Even with the computing power of a supercomputer, breaking VMPilot in parallel becomes a daunting challenge.

Experience the power of VMPilot and fortify your software against reverse engineering threats.

# Dependencies
- [CMake](https://cmake.org/download/) (3.26 or higher)
- Supporting C++17 or higher compiler
- OpenSSL 3.1.1
- autotools (autoconf, automake, and libtool) for retdec

### Underlining Dependencies
It would be fetched automatically by CPM(CMake Package Manager) during the build process.
- [nlohmann/json](https://github.com/nlohmann/json)
- [25077667/retdec](https://github.com/25077667/retdec) for SDK
    > It's our patched version of avast/retdec, which is used to decompile the target binary.

## Optional Dependencies
- [Ninja](https://github.com/ninja-build/ninja)
- [mold](https://github.com/rui314/mold)

# Build
Release build
```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -G Ninja -DCMAKE_LINKER=mold
ninja
```

Debug build
```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug -G Ninja -DCMAKE_LINKER=mold
ninja
```

# Documentation

TBW
