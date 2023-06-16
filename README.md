# VMPilot
A modern C++ written virtual machine SDK.

We are aiming for protect your software from reverse engineering. Moreover, it is easy to use and integrate into your project. 

This VMPilot is secure by design, and it is not a black box. You can easily understand how it works but it is hard to break it.

We introduce modern cryptography and obfuscation techniques to protect your software. Even if you had super-computer, it is hard to break it in parallel.

# Dependencies
- [CMake](https://cmake.org/download/) (3.26 or higher)
- Supporting C++17 or higher compiler

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
