#include <elf_seg.hpp>
#include <utilities.hpp>

#include <cstdint>
#include <fstream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <spdlog/spdlog.h>
#include <elfio/elf_types.hpp>
#include <elfio/elfio.hpp>
#include <elfio/elfio_dump.hpp>

namespace {
uint64_t get_vmp_addr(const ELFIO::symbol_section_accessor& accessor,
                      const std::string& signature) noexcept {

    auto size = accessor.get_symbols_num();
    for (size_t i = 0; i < size; i++) {
        ELFIO::Elf64_Addr value;
        std::string name;
        ELFIO::Elf_Xword size;
        unsigned char bind;
        unsigned char type;
        ELFIO::Elf_Half section_index;
        unsigned char other;

        accessor.get_symbol(i, name, value, size, bind, type, section_index,
                            other);

        if (name == signature)
            return value;
    }

    return 0;
}

std::pair<uint64_t, uint64_t> get_begin_end_addr(
    const ELFIO::elfio& elf_reader) noexcept {
    uint64_t vmp_begin_addr = 0;
    uint64_t vmp_end_addr = 0;
    // check if exist begin and end signature
    for (const auto& sec : elf_reader.sections) {
        if (ELFIO::SHT_SYMTAB != sec->get_type() &&
            ELFIO::SHT_DYNSYM != sec->get_type())
            continue;

        ELFIO::symbol_section_accessor symbols(elf_reader, sec.get());
        if (symbols.get_symbols_num() == 0)
            continue;

        vmp_begin_addr =
            get_vmp_addr(symbols, VMPilot::Common::BEGIN_VMPILOT_SIGNATURE);
        vmp_end_addr =
            get_vmp_addr(symbols, VMPilot::Common::END_VMPILOT_SIGNATURE);
        if (vmp_begin_addr != 0 && vmp_end_addr != 0)
            break;
    }

    // Begin and end should appear simultaneously or not appear at all
    if ((vmp_begin_addr == 0 && vmp_end_addr != 0) ||
        (vmp_begin_addr != 0 && vmp_end_addr == 0)) {
        spdlog::error(
            "Error: VMPilot_Begin and VMPilot_End should appear "
            "simultaneously or not appear at all");
        return {0, 0};
    }

    return {vmp_begin_addr, vmp_end_addr};
}
}  // namespace

void VMPilot::SDK::Segmentator::ELF_Segmentator::operator()() noexcept {

    ELFIO::elfio elf_reader;
    if (!elf_reader.load(m_filename)) {
        spdlog::error("Error: Can't find or process ELF file {}", m_filename);
        return;
    }

    const auto& [vmp_begin_addr, vmp_end_addr] = get_begin_end_addr(elf_reader);
    if (vmp_begin_addr == 0 && vmp_end_addr == 0) {
        spdlog::info("No VMPilot signatures found");
        return;
    }

    // TODO: cut binary within VMPilot_Begin and after VMPilot_End
}