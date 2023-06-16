# VMPilot
A modern C++ written virtual machine SDK

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

