#include <PEHandler.hpp>
#include <utilities.hpp>

#include <unordered_map>

#include <spdlog/spdlog.h>

using namespace VMPilot::SDK::Segmentator;

namespace {
namespace detail {
constexpr std::uint16_t NT_SHORT_NAME_LEN = 8;

struct PEParser {
    // Placeholder for PE parsing, not implemented
    // TODO: Add third party library for PE parsing
};

struct ImageSectionHeader {
    std::uint8_t Name[NT_SHORT_NAME_LEN];
    union {
        std::uint32_t PhysicalAddress;
        std::uint32_t VirtualSize;
    } Misc;
    std::uint32_t VirtualAddress;
    std::uint32_t SizeOfRawData;
    std::uint32_t PointerToRawData;
    std::uint32_t PointerToRelocations;
    std::uint32_t PointerToLinenumbers;
    std::uint16_t NumberOfRelocations;
    std::uint16_t NumberOfLinenumbers;
    std::uint32_t Characteristics;
};
}  // namespace detail
}  // namespace

struct PEFileHandlerStrategy::Impl {
    detail::PEParser pe_handler;
    std::unordered_map<std::string, detail::ImageSectionHeader> section_table;
    uint64_t vmp_begin_addr = -1;
    uint64_t vmp_end_addr = -1;
    uint64_t text_base_addr = -1;
};

std::unique_ptr<PEFileHandlerStrategy::Impl>
VMPilot::SDK::Segmentator::make_pe_impl([[maybe_unused]]  // NOLINT
                                        const std::string& file_name) {
    auto impl = std::make_unique<PEFileHandlerStrategy::Impl>();
    // Placeholder for PE parsing, not implemented

    return impl;
}

PEFileHandlerStrategy::PEFileHandlerStrategy(const std::string& file_name)
    : pImpl(make_pe_impl(file_name)) {
    spdlog::error("PEFileHandlerStrategy not implemented");
}

PEFileHandlerStrategy::~PEFileHandlerStrategy() {}

std::pair<uint64_t, uint64_t>
PEFileHandlerStrategy::doGetBeginEndAddr() noexcept {
    spdlog::error("PEFileHandlerStrategy::doGetBeginEndAddr not implemented");
    return std::make_pair(-1, -1);
}

std::vector<uint8_t> PEFileHandlerStrategy::doGetTextSection() noexcept {
    spdlog::error("PEFileHandlerStrategy::doGetTextSection not implemented");
    return std::vector<uint8_t>();
}

uint64_t PEFileHandlerStrategy::doGetTextBaseAddr() noexcept {
    spdlog::error("PEFileHandlerStrategy::doGetTextBaseAddr not implemented");
    return -1;
}

NativeSymbolTable PEFileHandlerStrategy::doGetNativeSymbolTable() noexcept {
    spdlog::error(
        "PEFileHandlerStrategy::doGetNativeSymbolTable not implemented");
    return NativeSymbolTable();
}

std::pair<uint64_t, uint64_t>
PEFileHandlerStrategy::doGetBeginEndAddrIntl() noexcept {
    spdlog::error(
        "PEFileHandlerStrategy::doGetBeginEndAddrIntl not implemented");
    return std::make_pair(-1, -1);
}

std::vector<uint8_t> PEFileHandlerStrategy::doGetTextSectionIntl() noexcept {
    spdlog::error(
        "PEFileHandlerStrategy::doGetTextSectionIntl not implemented");
    return std::vector<uint8_t>();
}

std::vector<uint8_t> PEFileHandlerStrategy::getSectionData(
    [[maybe_unused]]  // NOLINT
    const std::string& sectionName) noexcept {
    spdlog::error("PEFileHandlerStrategy::getSectionData not implemented");
    return std::vector<uint8_t>();
}
