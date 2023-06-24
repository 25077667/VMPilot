# VMPilot: A Modern C++ Virtual Machine SDK

VMPilot is an advanced virtual machine software development kit (SDK) implemented in C++. **Secure by design**, VMPilot is specifically engineered to safeguard your software from reverse engineering. Offering seamless integration and ease of use for your projects, VMPilot sets a new standard for software protection.

Unlike traditional black box solutions, VMPilot is built with transparency in mind. Its inner workings are easily understandable, yet formidable to crack. By incorporating modern cryptography and obfuscation techniques, your software is shielded against potential attacks. Even with the computing power of a supercomputer, breaking VMPilot in parallel becomes a daunting challenge.

Experience the power of VMPilot and fortify your software against reverse engineering threats.

## Expected Usage
Similar to VMProtect:
```cpp
#include <vmpilot/sdk.hpp>

template <typename T>
T square(T x) {
    VMPilot_Begin(__FUNCTION__);
    auto result = x * x;
    VMPilot_End(__FUNCTION__);
    return result;
}
```

Output:
```asm
square:
    push rbp
    call    _Z13VMPilot_BeginPKc    ; VMPilot_Begin(__FUNCTION__);
    ... garbage code ...
    ... garbage code ...
    ... garbage code ...
    call    _Z11VMPilot_EndPKc      ; VMPilot_End(__FUNCTION__);
    pop rbp
    ret
```

# Dependencies
- [CMake](https://cmake.org/download/) (3.20 or higher)
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

# Ongoing Projects

The following projects are currently in progress and will be released in the future.

You are welcome to contribute to these projects, and if you have any questions or need 
further information about the details, please feel free to contact me.


- [ ] VMPilot SDK
    - [ ] Protect Region Segmentator
        > Identify the protected region in the target binary and save them into different files.
    - [ ] Protect Region Obfuscator
        > LLVM IR Obfuscator
    - [ ] Protect Region Encryptor
        > Use the [common](common/) library to encrypt the protected region.
    - [ ] Native code decompiler (see [sdk/include/bytecode_compiler/bytecode_compiler.hpp](sdk/include/bytecode_compiler/bytecode_compiler.hpp))
        - [ ] x86 support
            > Finish this first.
            > It takes a segmented native x86 binary and decompiles it to LLVM IR.
        - [ ] x86_64 support
        - [ ] Support for other architectures
            - [ ] ARM, ARM64
    - [ ] LLVM IR Backend
        > Get `bytecode_compiler`'s output, LLVM IR, and compile it to our bytecode.
    - [ ] Post-process assembler
        > Assemble encrypted bytecode and native code into a single binary.

- [ ] VMPilot Loader
    - [ ] ELF loader
    - [ ] PE loader
    - [ ] Mach-O loader

- [ ] VMPilot runtime
    - [ ] VMPilot VM

- Miscellaneous
    - [ ] Migrate OpenSSL to [Botan](https://github.com/randombit/botan)
        - [ ] Remove OpenSSL dependency
        - [ ] CMake CPM support
    - [ ] Documentation
    - [ ] Unit tests
    - [ ] CI/CD

# Documentation and Known Issues

Please refer to [wiki](/wiki) for more information.

# Flowchart
![](/assets/outline.svg)