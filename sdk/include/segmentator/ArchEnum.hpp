#ifndef __SDK_ARCH_ENUM_HPP__
#define __SDK_ARCH_ENUM_HPP__
#pragma once
#include <cstdint>

namespace VMPilot::SDK::Segmentator {
enum class Arch : uint8_t {
    ARM,
    ARM64,
    MIPS,
    X86,
    PPC,
    SPARC,
    SYSZ,
    XCORE,
    M68K,
    TMS320C64X,
    M680X,
    EVM,
    MOS65XX,
    WASM,
    BPF,
    RISCV,
    SH,
    TRICORE,
    MAX,
    ALL,
};
}

#endif  // __SDK_ARCH_ENUM_HPP__