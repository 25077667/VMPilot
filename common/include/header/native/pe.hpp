#ifndef __COMMON_HEADER_NATIVE_PE_HPP__
#define __COMMON_HEADER_NATIVE_PE_HPP__
#pragma once

#include <cstdint>

namespace VMPilot::Common::Header {
struct PE32_Ehdr {
    uint16_t Machine;
    uint16_t NumberOfSections;
    uint32_t TimeDateStamp;
    uint32_t PointerToSymbolTable;
    uint32_t NumberOfSymbols;
    uint16_t SizeOfOptionalHeader;
    uint16_t Characteristics;
};

struct PE64_Ehdr {
    uint16_t Machine;
    uint16_t NumberOfSections;
    uint32_t TimeDateStamp;
    uint32_t PointerToSymbolTable;
    uint32_t NumberOfSymbols;
    uint16_t SizeOfOptionalHeader;
    uint16_t Characteristics;
};

union PEHeader {
    PE32_Ehdr pe32;
    PE64_Ehdr pe64;
};

}  // namespace VMPilot::Common::Header

#endif  // __COMMON_HEADER_NATIVE_PE_HPP__
