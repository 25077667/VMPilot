#include <Strategy.hpp>

#include <string>
#include <utility>

#include <spdlog/spdlog.h>
using namespace VMPilot::SDK::Segmentator;

std::pair<uint64_t, uint64_t>
FileHandlerStrategy::doGetBeginEndAddr() noexcept {
    spdlog::error("FileHandlerStrategy::doGetBeginEndAddr not implemented");
    return std::make_pair(-1, -1);
}

std::vector<uint8_t> FileHandlerStrategy::doGetTextSection() noexcept {
    spdlog::error("FileHandlerStrategy::doGetTextSection not implemented");
    return std::vector<uint8_t>();
}

uint64_t FileHandlerStrategy::doGetTextBaseAddr() noexcept {
    spdlog::error("FileHandlerStrategy::doGetTextBaseAddr not implemented");
    return -1;
}

NativeSymbolTable FileHandlerStrategy::doGetNativeSymbolTable() noexcept {
    spdlog::error(
        "FileHandlerStrategy::doGetNativeSymbolTable not implemented");
    return NativeSymbolTable();
}

bool ArchHandlerStrategy::doLoad(const std::vector<uint8_t>& code,
                                 const uint64_t base_addr) {
    // Emitting error message, not implemented
    spdlog::error(
        "ArchHandlerStrategy::doLoad not implemented: code size: {}, "
        "base_addr: {}",
        code.size(), base_addr);

    return false;
}

std::vector<std::unique_ptr<NativeFunctionBase>>
ArchHandlerStrategy::doGetNativeFunctions(
    [[maybe_unused]] const NativeSymbolTable& native_symbol_table) {
    spdlog::error("ArchHandlerStrategy::doGetNativeFunctions not implemented");
    return std::vector<std::unique_ptr<NativeFunctionBase>>();
}
