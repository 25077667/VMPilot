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

void ArchHandlerStrategy::doHandleArchitecture(const std::string& archType) {
    // Emitting error message, not implemented
    spdlog::error(
        "ArchHandlerStrategy::doHandleArchitecture not implemented, got arch: "
        "{}",
        archType);
}
