#include <ELFHandler.hpp>
#include <ELFSectionViewer.hpp>
#include <utilities.hpp>

#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include <spdlog/spdlog.h>

using namespace VMPilot::SDK::Segmentator;

struct ELFHandler::Impl {
    ELFIO::elfio reader;
    std::unordered_map<std::string, ELFSectionViewer> section_table;
    uint64_t vmp_begin_addr = -1;
    uint64_t vmp_end_addr = -1;
    uint64_t text_base_addr = -1;
};

std::unique_ptr<ELFHandler::Impl> VMPilot::SDK::Segmentator::make_elf_impl(
    const std::string& file_name) {
    auto impl = std::make_unique<ELFHandler::Impl>();
    if (!impl->reader.load(file_name)) {
        throw std::runtime_error("File not found or it is not an ELF file");
    }

    // Cache all section accessors in the section_table
    // This allows us to access the section_accessor by the section name directly
    for (auto&& section : impl->reader.sections) {
        impl->section_table[section->get_name()] =
            ELFSectionViewer(section.get());
    }

    return impl;
}

ELFHandler::ELFHandler(const std::string& file_name)
    : pImpl(make_elf_impl(file_name)) {}

ELFHandler::~ELFHandler() {
    pImpl->section_table.clear();
}

std::pair<uint64_t, uint64_t> ELFHandler::getBeginEndAddr() noexcept {
    if (pImpl->vmp_begin_addr == static_cast<uint64_t>(-1) ||
        pImpl->vmp_end_addr == static_cast<uint64_t>(-1)) {
        const auto& [begin_addr, end_addr] = doGetBeginEndAddr();
        pImpl->vmp_begin_addr = begin_addr;
        pImpl->vmp_end_addr = end_addr;
    }

    return {pImpl->vmp_begin_addr, pImpl->vmp_end_addr};
}

std::vector<uint8_t> ELFHandler::getTextSection() noexcept {
    const auto& chunk = this->doGetTextSection();
    if (chunk.empty()) {
        spdlog::error("Error: Could not find the .text section");
        return {};
    }
    return chunk;
}

uint64_t ELFHandler::getTextBaseAddr() noexcept {
    if (pImpl->text_base_addr == static_cast<uint64_t>(-1)) {
        const auto& text_section = pImpl->section_table.find(".text");
        if (text_section == pImpl->section_table.end()) {
            spdlog::error("Error: Could not find the .text section");
            return -1;
        }

        pImpl->text_base_addr =
            text_section->second.getSection()->get_address();
    }

    return pImpl->text_base_addr;
}

uint64_t ELFHandler::getEntryIndex(const std::string& signature) noexcept {
    const auto& dynsym = pImpl->section_table.find(".dynsym");
    if (dynsym == pImpl->section_table.end()) {
        return -1;
    }

    auto section = dynsym->second.getSection();
    if (!section) {
        spdlog::error("Failed to get the .dynsym section");
        return -1;
    }

    ELFIO::elfio& reader = pImpl->reader;  // NOLINT
    ELFIO::symbol_section_accessor accessor(reader, section);
    auto size = accessor.get_symbols_num();
    for (size_t i = 0; i < size; ++i) {
        std::string name;
        ELFIO::Elf64_Addr value;
        ELFIO::Elf_Xword size;
        unsigned char bind;
        unsigned char type;
        ELFIO::Elf_Half section_index;
        unsigned char other;

        if (!accessor.get_symbol(i, name, value, size, bind, type,
                                 section_index, other)) {
            spdlog::error("Failed to get the symbol at index {}", i);
            continue;
        }

        if (name == signature) {
            return i;
        }
    }

    return -1;
}

uint64_t ELFHandler::getRelapltIdx(uint64_t dynsym_idx) noexcept {
    // 32-bit is .rel.plt, 64-bit is .rela.plt
    static const char* relaplt_name[] = {".rel.plt", ".rela.plt"};
    const int& is_64_bit = pImpl->reader.get_class() == ELFIO::ELFCLASS64;

    const auto& relaplt = pImpl->section_table.find(relaplt_name[is_64_bit]);
    if (relaplt == pImpl->section_table.end()) {
        return -1;
    }

    auto section = relaplt->second.getSection();
    if (!section) {
        spdlog::error("Failed to get the .rela.plt section");
        return -1;
    }

    const auto& reader = pImpl->reader;  // NOLINT
    ELFIO::relocation_section_accessor accessor(reader, section);
    auto size = accessor.get_entries_num();
    for (size_t i = 0; i < size; ++i) {
        ELFIO::Elf64_Addr offset;
        ELFIO::Elf_Word symbol;
        ELFIO::Elf_Sxword addend;
        unsigned int type;

        if (!accessor.get_entry(i, offset, symbol, type, addend)) {
            spdlog::error("Failed to get the entry at index {}", i);
            continue;
        }

        if (symbol == dynsym_idx) {
            return i;
        }
    }

    return -1;
}

uint64_t ELFHandler::getPltAddr(uint64_t relaplt_idx) noexcept {
    const auto& plt = pImpl->section_table.find(".plt");
    if (plt == pImpl->section_table.end()) {
        return -1;
    }

    const auto& section = plt->second.getSection();
    if (!section) {
        spdlog::error("Failed to get the .plt section");
        return -1;
    }

    uint64_t alignment = section->get_addr_align();
    uint64_t plt_base_addr = section->get_address();

    return plt_base_addr + alignment * (relaplt_idx + 1);
}

std::pair<uint64_t, uint64_t> ELFHandler::doGetBeginEndAddr() noexcept {
    // Step 1: get the index of begin and end with getEntryIndex
    uint64_t begin_dynsym_idx =
        getEntryIndex(VMPilot::Common::BEGIN_VMPILOT_SIGNATURE);
    uint64_t end_dynsym_idx =
        getEntryIndex(VMPilot::Common::END_VMPILOT_SIGNATURE);
    if (begin_dynsym_idx == static_cast<uint64_t>(-1) ||
        end_dynsym_idx == static_cast<uint64_t>(-1)) {
        spdlog::error(
            "Error: Could not find the VMPilot signatures in the "
            ".dynsym section");
        return {-1, -1};
    }

    // Step 2: get the index of the .rela.plt section
    uint64_t begin_relaplt_idx = getRelapltIdx(begin_dynsym_idx);
    uint64_t end_relaplt_idx = getRelapltIdx(end_dynsym_idx);
    if (begin_relaplt_idx == static_cast<uint64_t>(-1) ||
        end_relaplt_idx == static_cast<uint64_t>(-1)) {
        spdlog::error(
            "Error: Could not find the VMPilot signatures in the "
            ".rela.plt or .rel.plt section");
        return {-1, -1};
    }

    // Step 3: get the base address and alignment of the .plt section
    uint64_t begin_addr = getPltAddr(begin_relaplt_idx);
    uint64_t end_addr = getPltAddr(end_relaplt_idx);
    if (begin_addr == static_cast<uint64_t>(-1) ||
        end_addr == static_cast<uint64_t>(-1)) {
        spdlog::error("Error: Could not find the .plt section");
        return {-1, -1};
    }

    return {begin_addr, end_addr};
}

std::vector<uint8_t> ELFHandler::doGetTextSection() noexcept {
    const auto& text_section_iter = pImpl->section_table.find(".text");
    if (text_section_iter == pImpl->section_table.end()) {
        return {};
    }

    auto section = text_section_iter->second.getSection();
    std::vector<uint8_t> text_section(section->get_size());
    std::memcpy(text_section.data(), section->get_data(), section->get_size());

    return text_section;
}
