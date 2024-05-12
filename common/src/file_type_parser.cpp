#include <file_type_parser.hpp>

#include <array>
#include <cstdint>
#include <fstream>
#include <stdexcept>
#include <unordered_map>
#include <utility>
#include <vector>

using namespace VMPilot::Common;

namespace {
namespace header {
struct Elf32_Ehdr {
    uint8_t e_ident[16];
    uint16_t e_type;
    uint16_t e_machine;
    uint32_t e_version;
    uint32_t e_entry;
    uint32_t e_phoff;
    uint32_t e_shoff;
    uint32_t e_flags;
    uint16_t e_ehsize;
    uint16_t e_phentsize;
    uint16_t e_phnum;
    uint16_t e_shentsize;
    uint16_t e_shnum;
    uint16_t e_shstrndx;
};

struct ELF64_Ehdr {
    uint8_t e_ident[16];
    uint16_t e_type;
    uint16_t e_machine;
    uint32_t e_version;
    uint64_t e_entry;
    uint64_t e_phoff;
    uint64_t e_shoff;
    uint32_t e_flags;
    uint16_t e_ehsize;
    uint16_t e_phentsize;
    uint16_t e_phnum;
    uint16_t e_shentsize;
    uint16_t e_shnum;
    uint16_t e_shstrndx;
};

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

union ElfHeader {
    Elf32_Ehdr elf32;
    ELF64_Ehdr elf64;
};

union PEHeader {
    PE32_Ehdr pe32;
    PE64_Ehdr pe64;
};

union MachOHeader {
    MachO_Ehdr macho;
};

}  // namespace header

namespace file_mode_table {
using Endian = bool;   // true for big endian, false for little endian
using Bits = uint8_t;  // 16, 32 or 64
constexpr auto LittleEndian = false;
constexpr auto BigEndian = true;

// (FileArch (uint16_t)) | (Endinaness (uint8_t)) | (Bits (uint8_t))
using MetaMode = uint32_t;

#define MODE(arch, endian, bits) \
    ((static_cast<uint16_t>(arch) << 16) | (endian << 8) | bits)

static const std::unordered_map<MetaMode, FileMode> mode_table = {
    {MODE(FileArch::ARM, LITTLE_ENDIAN, 32), FileMode::MODE_ARM},
    {MODE(FileArch::ARM64, LITTLE_ENDIAN, 64), FileMode::MODE_ARM},
    {MODE(FileArch::X86, LITTLE_ENDIAN, 16), FileMode::MODE_16},
    {MODE(FileArch::X86, LITTLE_ENDIAN, 32), FileMode::MODE_32},
    {MODE(FileArch::X86, LITTLE_ENDIAN, 64), FileMode::MODE_64},
    {MODE(FileArch::RISCV, LITTLE_ENDIAN, 32), FileMode::MODE_RISCV32},
    {MODE(FileArch::RISCV, LITTLE_ENDIAN, 64), FileMode::MODE_RISCV64},

    // TODO: complete this table
};

MetaMode lookup_mode(FileArch arch, Endian endian, Bits bits) noexcept {
    return MODE(arch, endian, bits);
}

#undef MODE
}  // namespace file_mode_table

namespace detail {
uint64_t get_file_size(const std::string& filename);
Magic get_magic_number(const std::string& filename);
FileFormat get_file_format(Magic magic);

FileArch get_file_arch(const std::string& filename, FileFormat format);
FileMode get_file_mode(const std::string& filename, FileFormat format);
ExecutableType get_executable_type(const std::string& filename,
                                   FileFormat format);

FileArch get_file_arch_elf(const std::string& filename);
FileMode get_file_mode_elf(const std::string& filename);
ExecutableType get_executable_type_elf(const std::string& filename);

FileArch get_file_arch_pe(const std::string& filename);
FileMode get_file_mode_pe(const std::string& filename);
ExecutableType get_executable_type_pe(const std::string& filename);

FileArch get_file_arch_macho(const std::string& filename);
FileMode get_file_mode_macho(const std::string& filename);
ExecutableType get_executable_type_macho(const std::string& filename);

header::ElfHeader get_elf_header(const std::string& filename);
header::PEHeader get_pe_header(const std::string& filename);
header::MachOHeader get_macho_header(const std::string& filename);

}  // namespace detail
}  // namespace

FileMetadata VMPilot::Common::get_file_metadata(const std::string& filename) {
    FileMetadata metadata;
    metadata.filename = filename;
    metadata.size = detail::get_file_size(filename);
    metadata.magic = detail::get_magic_number(filename);
    metadata.format = detail::get_file_format(metadata.magic);

    metadata.arch = detail::get_file_arch(filename, metadata.format);
    metadata.mode = detail::get_file_mode(filename, metadata.format);
    metadata.execType = detail::get_executable_type(filename, metadata.format);

    return metadata;
}

uint64_t detail::get_file_size(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file) {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    return file.tellg();
}

Magic detail::get_magic_number(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error(
            "Failed to open file for magic number reading: " + filename);
    }
    uint32_t magic;
    if (file.read(reinterpret_cast<char*>(&magic), sizeof(magic))) {
        return static_cast<Magic>(magic);
    }
    // Should not reach here
    throw std::runtime_error("Failed to read magic number from file: " +
                             filename);
}

FileFormat detail::get_file_format(Magic magic) {
    switch (magic) {
        case Magic::ELF:
            return FileFormat::ELF;
        case Magic::PE:
            return FileFormat::PE;
        case Magic::MachO32:
            return FileFormat::MachO;
        case Magic::MachO64:
            return FileFormat::MachO;
        default:
            throw std::runtime_error("Unsupported file format");
    }
}

FileArch detail::get_file_arch(const std::string& filename, FileFormat format) {
    switch (format) {
        case FileFormat::ELF:
            return get_file_arch_elf(filename);
        case FileFormat::PE:
            return get_file_arch_pe(filename);
        case FileFormat::MachO:
            return get_file_arch_macho(filename);
        default:
            throw std::runtime_error("Unsupported file format");
    }
}

FileMode detail::get_file_mode(const std::string& filename, FileFormat format) {
    switch (format) {
        case FileFormat::ELF:
            return get_file_mode_elf(filename);
        case FileFormat::PE:
            return get_file_mode_pe(filename);
        case FileFormat::MachO:
            return get_file_mode_macho(filename);
        default:
            throw std::runtime_error("Unsupported file format");
    }
}

ExecutableType detail::get_executable_type(const std::string& filename,
                                           FileFormat format) {
    switch (format) {
        case FileFormat::ELF:
            return get_executable_type_elf(filename);
        case FileFormat::PE:
            return get_executable_type_pe(filename);
        case FileFormat::MachO:
            return get_executable_type_macho(filename);
        default:
            throw std::runtime_error("Unsupported file format");
    }
}

header::ElfHeader detail::get_elf_header(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error(
            "Failed to open file for ELF header reading: " + filename);
    }

    header::ElfHeader elf_header;
    if (!file.read(reinterpret_cast<char*>(&elf_header), sizeof(elf_header))) {
        throw std::runtime_error("Failed to read ELF header from file: " +
                                 filename);
    }

    return elf_header;
}

header::PEHeader detail::get_pe_header(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Failed to open file for PE header reading: " +
                                 filename);
    }

    header::PEHeader pe_header;
    if (!file.read(reinterpret_cast<char*>(&pe_header), sizeof(pe_header))) {
        throw std::runtime_error("Failed to read PE header from file: " +
                                 filename);
    }

    return pe_header;
}

header::MachOHeader detail::get_macho_header(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error(
            "Failed to open file for Mach-O header reading: " + filename);
    }

    header::MachOHeader macho_header;
    if (!file.read(reinterpret_cast<char*>(&macho_header),
                   sizeof(macho_header))) {
        throw std::runtime_error("Failed to read Mach-O header from file: " +
                                 filename);
    }

    return macho_header;
}

FileArch detail::get_file_arch_elf(const std::string& filename) {
    const auto& elf_header = get_elf_header(filename);

    static const std::unordered_map<uint16_t, VMPilot::Common::FileArch>
        arch_table = {
            {0x00, VMPilot::Common::FileArch::ALL},
            {0x01, VMPilot::Common::FileArch::ARM},
            {0x02, VMPilot::Common::FileArch::SPARC},
            {0x03, VMPilot::Common::FileArch::X86},
            {0x04, VMPilot::Common::FileArch::M68K},
            {0x08, VMPilot::Common::FileArch::MIPS},
            {0x14, VMPilot::Common::FileArch::PPC},
            {0x16, VMPilot::Common::FileArch::SYSZ},
            {0x28, VMPilot::Common::FileArch::ARM64},
            {0x2A, VMPilot::Common::FileArch::XCORE},
            {0x2C, VMPilot::Common::FileArch::SPARC},
            {0x32, VMPilot::Common::FileArch::TMS320C64X},
            {0x35, VMPilot::Common::FileArch::M680X},
            {0x3B, VMPilot::Common::FileArch::EVM},
            {0x3E, VMPilot::Common::FileArch::MOS65XX},
            {0xF3, VMPilot::Common::FileArch::RISCV},
            {0x28, VMPilot::Common::FileArch::SH},
            {0x2B, VMPilot::Common::FileArch::TRICORE},
        };

    auto it = arch_table.find(elf_header.elf32.e_machine);
    if (it == arch_table.end()) {
        throw std::runtime_error("Unsupported ELF machine type");
    }

    return it->second;
}

FileMode detail::get_file_mode_elf(const std::string& filename) {
    const auto& elf_header = get_elf_header(filename);

    const auto& key = file_mode_table::lookup_mode(
        get_file_arch_elf(filename), elf_header.elf32.e_ident[5] == 1,
        elf_header.elf32.e_ident[4] == 2 ? 64 : 32);

    auto it = file_mode_table::mode_table.find(key);
    if (it == file_mode_table::mode_table.end()) {
        throw std::runtime_error("Unsupported ELF mode");
    }

    return it->second;
}

ExecutableType detail::get_executable_type_elf(const std::string& filename) {
    const auto& elf_header = get_elf_header(filename);

    if (elf_header.elf32.e_type == 2) {
        return ExecutableType::DYNAMIC_LIBRARY;
    } else if (elf_header.elf32.e_type == 3) {
        return ExecutableType::STATIC_LIBRARY;
    } else if (elf_header.elf32.e_type == 1) {
        return ExecutableType::EXECUTABLE;
    } else if (elf_header.elf32.e_type == 0) {
        return ExecutableType::OBJECT_FILE;
    } else {
        return ExecutableType::UNKNOWN;
    }
}

FileArch detail::get_file_arch_pe(const std::string& filename) {
    const auto& pe_header = get_pe_header(filename);

    static const std::unordered_map<uint16_t, VMPilot::Common::FileArch>
        arch_table = {
            {0x014C, VMPilot::Common::FileArch::X86},
            {0x8664, VMPilot::Common::FileArch::X86},
            {0x01C0, VMPilot::Common::FileArch::ARM},
            {0xaa64, VMPilot::Common::FileArch::ARM64},
        };

    auto it = arch_table.find(pe_header.pe32.Machine);
    if (it == arch_table.end()) {
        throw std::runtime_error("Unsupported PE machine type");
    }

    return it->second;
}

FileMode detail::get_file_mode_pe(const std::string& filename) {
    const auto& pe_header = get_pe_header(filename);

    const auto& key = file_mode_table::lookup_mode(
        get_file_arch_pe(filename), false, pe_header.pe32.SizeOfOptionalHeader);

    auto it = file_mode_table::mode_table.find(key);
    if (it == file_mode_table::mode_table.end()) {
        throw std::runtime_error("Unsupported PE mode");
    }

    return it->second;
}

ExecutableType detail::get_executable_type_pe(const std::string& filename) {
    const auto& pe_header = get_pe_header(filename);

    if (pe_header.pe32.Characteristics & 0x2000) {
        return ExecutableType::DYNAMIC_LIBRARY;
    } else if (pe_header.pe32.Characteristics & 0x0002) {
        return ExecutableType::STATIC_LIBRARY;
    } else if (pe_header.pe32.Characteristics & 0x0001) {
        return ExecutableType::EXECUTABLE;
    } else {
        return ExecutableType::UNKNOWN;
    }
}

FileArch detail::get_file_arch_macho(const std::string& filename) {
    const auto& macho_header = get_macho_header(filename);

    static const std::unordered_map<uint32_t, VMPilot::Common::FileArch>
        arch_table = {
            {0x00000007, VMPilot::Common::FileArch::X86},
            {0x01000007, VMPilot::Common::FileArch::X86},
            {0x0000000C, VMPilot::Common::FileArch::ARM},
            {0x0100000C, VMPilot::Common::FileArch::ARM},
            {0x0000000E, VMPilot::Common::FileArch::ARM64},
            {0x0100000E, VMPilot::Common::FileArch::ARM64},
        };

    auto it = arch_table.find(macho_header.macho.cputype);
    if (it == arch_table.end()) {
        throw std::runtime_error("Unsupported Mach-O machine type");
    }

    return it->second;
}

FileMode detail::get_file_mode_macho(const std::string& filename) {
    const auto& macho_header = get_macho_header(filename);
    const auto& is_64_bit =
        macho_header.macho.magic == static_cast<uint32_t>(Magic::MachO64);

    const auto& key = file_mode_table::lookup_mode(
        get_file_arch_macho(filename), false, is_64_bit ? 64 : 32);

    auto it = file_mode_table::mode_table.find(key);
    if (it == file_mode_table::mode_table.end()) {
        throw std::runtime_error("Unsupported Mach-O mode");
    }

    return it->second;
}

ExecutableType detail::get_executable_type_macho(const std::string& filename) {
    const auto& macho_header = get_macho_header(filename);

    if (macho_header.macho.filetype == 0x02) {
        return ExecutableType::DYNAMIC_LIBRARY;
    } else if (macho_header.macho.filetype == 0x08) {
        return ExecutableType::STATIC_LIBRARY;
    } else if (macho_header.macho.filetype == 0x01) {
        return ExecutableType::EXECUTABLE;
    } else {
        return ExecutableType::UNKNOWN;
    }
}
