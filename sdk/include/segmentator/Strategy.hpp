#ifndef __SDK_SEGMENTATOR_STRATEGY_HPP__
#define __SDK_SEGMENTATOR_STRATEGY_HPP__
#pragma once

#include <cstdint>
#include <string>
#include <utility>
#include <vector>

namespace VMPilot::SDK::Segmentator {

// Strategy for file handling
class FileHandlerStrategy {
   protected:
    /**
     * @brief Get the begin and end address of the VMPilot signatures.
     */
    virtual std::pair<uint64_t, uint64_t> doGetBeginEndAddr() noexcept;
    /**
     * @brief Get the entire chuck of the .text section.
     */
    virtual std::vector<uint8_t> doGetTextSection() noexcept;

    virtual uint64_t doGetTextBaseAddr() noexcept;

   public:
    virtual ~FileHandlerStrategy() = default;
    std::pair<uint64_t, uint64_t> getBeginEndAddr() {
        return doGetBeginEndAddr();
    }
    std::vector<uint8_t> getTextSection() { return doGetTextSection(); }
    uint64_t getTextBaseAddr() { return doGetTextBaseAddr(); }
};

// Strategy for architecture handling
class ArchHandlerStrategy {
   protected:
    virtual void doHandleArchitecture(const std::string& archType);

   public:
    virtual ~ArchHandlerStrategy() = default;
    void handleArchitecture(const std::string& archType) {
        doHandleArchitecture(archType);
    }
};

}  // namespace VMPilot::SDK::Segmentator

#endif  // __SDK_SEGMENTATOR_STRATEGY_HPP__