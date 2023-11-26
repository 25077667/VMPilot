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
uint64_t get_entry_index(const ELFIO::symbol_section_accessor& accessor,
                         const std::string& signature) noexcept {
    auto size = accessor.get_symbols_num();
    for (size_t i = 0; i < size; i++) {
        ELFIO::Elf64_Addr value = -1;
        std::string name;
        ELFIO::Elf_Xword size = 0;
        unsigned char bind = 0;
        unsigned char type = 0;
        ELFIO::Elf_Half section_index = 0;
        unsigned char other = 0;

        accessor.get_symbol(i, name, value, size, bind, type, section_index,
                            other);
        if (name == signature)
            return i;
    }

    return -1;
}

std::pair<uint64_t, uint64_t> get_begin_end_idx_on_dynsym(
    const ELFIO::symbol_section_accessor& accessor) noexcept {

    if (accessor.get_symbols_num() == 0)
        return {0, 0};

    uint64_t vmp_begin_idx = 0;
    uint64_t vmp_end_idx = 0;

    vmp_begin_idx =
        get_entry_index(accessor, VMPilot::Common::BEGIN_VMPILOT_SIGNATURE);
    vmp_end_idx =
        get_entry_index(accessor, VMPilot::Common::END_VMPILOT_SIGNATURE);

    return {vmp_begin_idx, vmp_end_idx};
}

uint64_t dynsym_idx_to_relplt_idx(const ELFIO::elfio& elf_reader,
                                  uint64_t dynsym_idx) noexcept {
    uint64_t relplt_idx = 0;
    for (const auto& sec : elf_reader.sections) {
        if (ELFIO::SHT_REL != sec->get_type())
            continue;

        ELFIO::relocation_section_accessor rels(elf_reader, sec.get());
        auto size = rels.get_entries_num();
        for (size_t i = 0; i < size; i++) {
            ELFIO::Elf64_Addr offset = 0;
            ELFIO::Elf_Word symbol = 0;
            ELFIO::Elf_Word type = 0;
            int64_t addend = 0;

            rels.get_entry(i, offset, symbol, type, addend);

            if (symbol == dynsym_idx) {
                relplt_idx = i;
                break;
            }
        }
    }

    return relplt_idx;
}

/**
 * @brief Similar to nm -d -j .plt <filename> | grep VMPilot_Begin and VMPilot_End
 */
std::pair<uint64_t, uint64_t> get_begin_end_addr(
    const ELFIO::elfio& elf_reader) noexcept {
    uint64_t begin_dynsym_idx = 0;
    uint64_t end_dynsym_idx = 0;
    // check if exist begin and end signature
    for (const auto& sec : elf_reader.sections) {
        // the VMPilot signatures are only on the .dynsym section
        if (sec->get_name() != ".dynsym")
            continue;

        ELFIO::symbol_section_accessor symbols(elf_reader, sec.get());
        const auto& [begin_idx, end_idx] = get_begin_end_idx_on_dynsym(symbols);
        begin_dynsym_idx = begin_idx;
        end_dynsym_idx = end_idx;
        break;
    }

    // Begin and end should appear simultaneously or not appear at all
    if (begin_dynsym_idx == 0 || end_dynsym_idx == 0) {
        return {0, 0};
    }

    // get the index on .rel.plt section, which is the index of the .plt section
    // if there is an entry with index k on the .rel.plt section, which Info is vmp_begin_idx<a_type_byte>
    // than the index of the .plt section is k + 1
    uint64_t begin_relplt_idx =
        dynsym_idx_to_relplt_idx(elf_reader, begin_dynsym_idx);
    uint64_t end_relplt_idx =
        dynsym_idx_to_relplt_idx(elf_reader, end_dynsym_idx);

    // get the base address and alignment of the .plt section
    uint64_t alignment = 0;
    uint64_t plt_base_addr = 0;
    for (const auto& sec : elf_reader.sections) {
        if (ELFIO::SHT_PROGBITS != sec->get_type())
            continue;

        if (sec->get_name() == ".plt") {
            plt_base_addr = sec->get_address();
            alignment = sec->get_addr_align();
            break;
        }
    }

    // return plt + alignment + offset
    return {plt_base_addr + alignment * (begin_relplt_idx + 1),
            plt_base_addr + alignment * (end_relplt_idx + 1)};
}
}  // namespace

void VMPilot::SDK::Segmentator::ELF_Segmentator::segmentation() noexcept {

    ELFIO::elfio elf_reader;
    if (!elf_reader.load(m_filename)) {
        spdlog::error("Error: Can't find or process ELF file {}", m_filename);
        return;
    }

    const auto& [vmp_begin_addr, vmp_end_addr] = get_begin_end_addr(elf_reader);
    if (vmp_begin_addr == 0 || vmp_end_addr == 0) {
        spdlog::info("No VMPilot signatures found");
        return;
    }

    // TODO: cut binary within VMPilot_Begin and after VMPilot_End
}