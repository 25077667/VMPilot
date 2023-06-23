# VMPilot: A Modern C++ Virtual Machine SDK

VMPilot is an advanced virtual machine software development kit (SDK) implemented in C++. **Secure by design**, VMPilot is specifically engineered to safeguard your software from reverse engineering. Offering seamless integration and ease of use for your projects, VMPilot sets a new standard for software protection.

Unlike traditional black box solutions, VMPilot is built with transparency in mind. Its inner workings are easily understandable, yet formidable to crack. By incorporating modern cryptography and obfuscation techniques, your software is shielded against potential attacks. Even with the computing power of a supercomputer, breaking VMPilot in parallel becomes a daunting challenge.

Experience the power of VMPilot and fortify your software against reverse engineering threats.

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

We know there are some supporting issue for retdec on ARM64, and we would deal with it in the future.
```    
[2/2] Linking CXX executable bin/retdec-bin2pat
FAILED: bin/retdec-bin2pat 
: && ... some compile error ... :
Undefined symbols for architecture arm64:
  "__ZN3re23RE23Set3AddERKNS_11StringPieceEPNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE", referenced from:
      __ZN7yaramod12ParserDriver10initializeEv in libyaramod.a(parser_driver.cpp.o)
  "__ZN3re23RE2C1ERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE", referenced from:
      __ZN3pog6ParserIN7yaramod5ValueEEC1Ev in libyaramod.a(parser_driver.cpp.o)
      __ZN7yaramod12ParserDriver10initializeEv in libyaramod.a(parser_driver.cpp.o)
  "__ZNK3re23RE23Set5MatchERKNS_11StringPieceEPSt6vectorIiSaIiEE", referenced from:
      __ZN3pog9TokenizerIN7yaramod5ValueEE10next_tokenEv in libyaramod.a(parser_driver.cpp.o)
  "__ZNKSt3__115basic_stringbufIcNS_11char_traitsIcEENS_9allocatorIcEEE3strEv", referenced from:
      __ZN10LogMessage5FlushEv in libpog_re2.a(set.cc.o)
      __ZN10LogMessage5FlushEv in libpog_re2.a(re2.cc.o)
      __ZN10LogMessage5FlushEv in libpog_re2.a(compile.cc.o)
      __ZN10LogMessage5FlushEv in libpog_re2.a(onepass.cc.o)
      __ZN10LogMessage5FlushEv in libpog_re2.a(bitstate.cc.o)
      __ZN10LogMessage5FlushEv in libpog_re2.a(dfa.cc.o)
      __ZN10LogMessage5FlushEv in libpog_re2.a(nfa.cc.o)
```

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
    - [ ] Protect Region Segamentator
        > Identify the protect region in the target binary, save them into different files.
    - [ ] Protect Region Obfuscator
        > LLVM IR Obfuscator
    - [ ] Protect Region Encryptor
        > Use the [common](common/) library to encrypt the protect region.
    - [ ] native code decomplier (see [sdk/include/bytecode_compiler/bytecode_compiler.hpp](sdk/include/bytecode_compiler/bytecode_compiler.hpp))
        - [ ] x86 support
            > Finish this first
            > It takes a segmented native x86 binary and decompile it to LLVM IR.
        - [ ] x86_64 support
        - [ ] More further architectures support
            - [ ] ARM, ARM64
    - [ ] LLVM IR Backend
        > Get `bytecode_compiler`'s output, LLVM IR, and compile it to our bytecode.
    - [ ] Post-process assembler
        >  Assemble encrypted bytecode and native code in to a single binary

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

# Documentation

TBW

# Flowchart
![](/assets/outline.svg)