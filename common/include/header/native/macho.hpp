#ifndef __COMMON_HEADER_NATIVE_MACHO_HPP__
#define __COMMON_HEADER_NATIVE_MACHO_HPP__
#pragma once

#include <cstdint>

namespace VMPilot::Common::Header {
struct MachO_Ehdr {
    uint32_t magic;
    uint32_t cputype;
    uint32_t cpusubtype;
    uint32_t filetype;
    uint32_t ncmds;
    uint32_t sizeofcmds;
    uint32_t flags;
    uint32_t reserved;
};

union MachOHeader {
    MachO_Ehdr macho;
};
}  // namespace VMPilot::Common::Header

#endif  // __COMMON_HEADER_NATIVE_MACHO_HPP__