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

namespace {
namespace detail {

/**
 * @brief Looks up a section in the cache table by name.
 *
 * This function searches for a section with the specified name in the cache table.
 *
 * @param table The cache table containing the sections.
 * @param name The name of the section to look up.
 * @return A pointer to the section if found, nullptr otherwise.
 */
ELFIO::section* lookup_cache_section_table(
    std::unordered_map<std::string,
                       VMPilot::SDK::Segmentator::ELFSectionViewer>& table,
    const std::string& name) noexcept;

/**
 * @brief Callback function type for handling ELF entries.
 *
 * This function type is used as a callback for handling ELF entries. It takes the following parameters:
 * - `std::string`: The name of the ELF entry.
 * - `ELFIO::Elf64_Addr`: The address of the ELF entry.
 * - `ELFIO::Elf_Xword`: The size of the ELF entry.
 * - `unsigned char`: The type of the ELF entry.
 * - `unsigned char`: The information of the ELF entry.
 * - `ELFIO::Elf_Half`: The section index of the ELF entry.
 * - `unsigned char`: The binding information of the ELF entry.
 *
 * The callback function can be used to perform custom operations on each ELF entry.
 */
using ELFEntryCallback = std::function<void(
    std::string, ELFIO::Elf64_Addr, ELFIO::Elf_Xword, unsigned char,
    unsigned char, ELFIO::Elf_Half, unsigned char)>;

/**
 * Enumerates the dynamic symbols in the given ELF section.
 *
 * @param reader The ELFIO object representing the ELF file.
 * @param section The ELF section containing the dynamic symbols.
 * @param callback The callback function to be called for each dynamic symbol.
 */
void enumerate_dynamic_symbols(ELFIO::elfio& reader, ELFIO::section* section,
                               ELFEntryCallback callback);
}  // namespace detail
}  // namespace

using namespace VMPilot::SDK::Segmentator;

struct ELFFileHandlerStrategy::Impl {
    ELFIO::elfio reader;
    std::unordered_map<std::string, ELFSectionViewer> section_table;
    NativeSymbolTable native_symbol_table;
    uint64_t vmp_begin_addr = -1;
    uint64_t vmp_end_addr = -1;
    uint64_t text_base_addr = -1;
};

std::unique_ptr<ELFFileHandlerStrategy::Impl>
VMPilot::SDK::Segmentator::make_elf_impl(const std::string& file_name) {
    auto impl = std::make_unique<ELFFileHandlerStrategy::Impl>();
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

ELFFileHandlerStrategy::ELFFileHandlerStrategy(const std::string& file_name)
    : pImpl(make_elf_impl(file_name)) {}

ELFFileHandlerStrategy::~ELFFileHandlerStrategy() {
    pImpl->section_table.clear();
}

std::pair<uint64_t, uint64_t>
ELFFileHandlerStrategy::doGetBeginEndAddr() noexcept {
    if (pImpl->vmp_begin_addr == static_cast<uint64_t>(-1) ||
        pImpl->vmp_end_addr == static_cast<uint64_t>(-1)) {
        const auto& [begin_addr, end_addr] = doGetBeginEndAddrIntl();
        pImpl->vmp_begin_addr = begin_addr;
        pImpl->vmp_end_addr = end_addr;
    }

    return {pImpl->vmp_begin_addr, pImpl->vmp_end_addr};
}

std::vector<uint8_t> ELFFileHandlerStrategy::doGetTextSection() noexcept {
    const auto& chunk = this->doGetTextSectionIntl();
    if (chunk.empty()) {
        spdlog::error("Error: Could not find the .text section");
        return {};
    }
    return chunk;
}

uint64_t ELFFileHandlerStrategy::doGetTextBaseAddr() noexcept {
    if (pImpl->text_base_addr == static_cast<uint64_t>(-1)) {
        auto text_section =
            detail::lookup_cache_section_table(pImpl->section_table, ".text");

        pImpl->text_base_addr = text_section->get_address();
    }

    return pImpl->text_base_addr;
}

NativeSymbolTable ELFFileHandlerStrategy::doGetNativeSymbolTable() noexcept {
    if (pImpl->native_symbol_table.empty()) {
        pImpl->native_symbol_table = doGetNativeSymbolTableIntl();
    }

    return pImpl->native_symbol_table;
}

uint64_t ELFFileHandlerStrategy::getEntryIndex(
    const std::string& signature) noexcept {

    auto dynsym_section =
        detail::lookup_cache_section_table(pImpl->section_table, ".dynsym");
    if (!dynsym_section) {
        spdlog::error("Failed to get the .dynsym section");
        return -1;
    }

    uint64_t result_index = -1;
    auto callback = [&result_index, &signature](
                        std::string name, ELFIO::Elf64_Addr value,
                        ELFIO::Elf_Xword size, unsigned char bind,
                        unsigned char type, ELFIO::Elf_Half section_index,
                        unsigned char other) mutable {
        if (name == signature) {
            result_index = section_index;
        }

        // Suppress unused variable warnings
        (void)value;
        (void)size;
        (void)bind;
        (void)type;
        (void)section_index;
        (void)other;
    };

    detail::enumerate_dynamic_symbols(pImpl->reader, dynsym_section, callback);
    return result_index;
}

uint64_t ELFFileHandlerStrategy::getRelapltIdx(uint64_t dynsym_idx) noexcept {
    // 32-bit is .rel.plt, 64-bit is .rela.plt
    static const char* relaplt_name[] = {".rel.plt", ".rela.plt"};
    const int& is_64_bit = pImpl->reader.get_class() == ELFIO::ELFCLASS64;

    auto relaplt_section = detail::lookup_cache_section_table(
        pImpl->section_table, relaplt_name[is_64_bit]);
    if (!relaplt_section) {
        spdlog::error("Failed to get the .rela.plt section");
        return -1;
    }

    const auto& reader = pImpl->reader;  // NOLINT
    ELFIO::relocation_section_accessor accessor(reader, relaplt_section);
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

uint64_t ELFFileHandlerStrategy::getPltAddr(uint64_t relaplt_idx) noexcept {
    const auto* plt_section =
        detail::lookup_cache_section_table(pImpl->section_table, ".plt");

    uint64_t alignment = plt_section->get_addr_align();
    uint64_t plt_base_addr = plt_section->get_address();

    return plt_base_addr + alignment * (relaplt_idx + 1);
}

std::pair<uint64_t, uint64_t>
ELFFileHandlerStrategy::doGetBeginEndAddrIntl() noexcept {
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

std::vector<uint8_t> ELFFileHandlerStrategy::doGetTextSectionIntl() noexcept {
    const auto& text_section_iter = pImpl->section_table.find(".text");
    if (text_section_iter == pImpl->section_table.end()) {
        return {};
    }

    auto section = text_section_iter->second.getSection();
    std::vector<uint8_t> text_section(section->get_size());
    std::memcpy(text_section.data(), section->get_data(), section->get_size());

    return text_section;
}

NativeSymbolTable
ELFFileHandlerStrategy::doGetNativeSymbolTableIntl() noexcept {
    // get all symbols from the .dynsym section and .symtab
    // and return them as a NativeSymbolTable
    NativeSymbolTable symbol_table;

    auto dynsym_section =
        detail::lookup_cache_section_table(pImpl->section_table, ".dynsym");
    if (!dynsym_section) {
        spdlog::error("Failed to get the .dynsym section");
        return symbol_table;
    }

    auto dynsym_section_callback =
        [&symbol_table](std::string name, ELFIO::Elf64_Addr value,
                        ELFIO::Elf_Xword size, unsigned char bind,
                        unsigned char type, ELFIO::Elf_Half section_index,
                        unsigned char other) mutable {
            symbol_table.emplace_back(
                NativeSymbolTableEntry{name, value, size, SymbolSource::DYNAMIC,
                                       static_cast<SymbolType>(type),
                                       static_cast<SymbolVisibility>(other),
                                       static_cast<SymbolBinding>(bind),
                                       static_cast<uint64_t>(section_index),
                                       static_cast<uint64_t>(section_index)});
        };
    detail::enumerate_dynamic_symbols(pImpl->reader, dynsym_section,
                                      dynsym_section_callback);

    auto symtab_section =
        detail::lookup_cache_section_table(pImpl->section_table, ".symtab");
    if (!symtab_section) {
        spdlog::error("Failed to get the .symtab section");
        return symbol_table;
    }

    auto symtab_section_callback =
        [&symbol_table](std::string name, ELFIO::Elf64_Addr value,
                        ELFIO::Elf_Xword size, unsigned char bind,
                        unsigned char type, ELFIO::Elf_Half section_index,
                        unsigned char other) mutable {
            symbol_table.emplace_back(
                NativeSymbolTableEntry{name, value, size, SymbolSource::STATIC,
                                       static_cast<SymbolType>(type),
                                       static_cast<SymbolVisibility>(other),
                                       static_cast<SymbolBinding>(bind),
                                       static_cast<uint64_t>(section_index),
                                       static_cast<uint64_t>(section_index)});
        };
    detail::enumerate_dynamic_symbols(pImpl->reader, symtab_section,
                                      symtab_section_callback);
    return symbol_table;
}

ELFIO::section* detail::lookup_cache_section_table(
    std::unordered_map<std::string,
                       VMPilot::SDK::Segmentator::ELFSectionViewer>& table,
    const std::string& name) noexcept {
    auto section = table.find(name);
    if (section == table.end()) {
        spdlog::error("Failed to get the {} section", name);
        return nullptr;
    }

    return section->second.getSection();
}

void detail::enumerate_dynamic_symbols(ELFIO::elfio& reader,
                                       ELFIO::section* section,
                                       detail::ELFEntryCallback callback) {

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

        callback(name, value, size, bind, type, section_index, other);
    }
}