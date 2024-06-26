#ifndef __SDK_MACHO_HANDLER_HPP__
#define __SDK_MACHO_HANDLER_HPP__
#pragma once

#include <Strategy.hpp>

#include <cstdint>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace VMPilot::SDK::Segmentator {
class MachOFileHandlerStrategy : public FileHandlerStrategy {
   private:
    struct Impl;
    std::unique_ptr<Impl> pImpl;
    // Private method to create an instance of the implementation
    friend std::unique_ptr<Impl> make_macho_impl(const std::string& filename);

   public:
    explicit MachOFileHandlerStrategy(const std::string& filename);
    virtual ~MachOFileHandlerStrategy();

   protected:
    /**
     * @brief Get the begin and end address of the VMPilot signatures.
     */
    virtual std::pair<uint64_t, uint64_t> doGetBeginEndAddr() noexcept override;

    /**
     * @brief Get the entire chunk of the executable code section (usually __TEXT,__text).
     */
    virtual std::vector<uint8_t> doGetTextSection() noexcept override;

    /**
     * @brief Get the base address of the executable code section (usually __TEXT,__text).
     */
    virtual uint64_t doGetTextBaseAddr() noexcept override;

   private:
    // Internal implementations that perform the actual logic
    std::pair<uint64_t, uint64_t> doGetBeginEndAddrIntl() noexcept;
    std::vector<uint8_t> doGetTextSectionIntl() noexcept;

    /**
     * Parses headers to locate and return specific Mach-O section data.
     *
     * @param sectionName The name of the section to retrieve (including segment name like __TEXT,__text).
     * @return The data of the section if found, or an empty vector if not.
     */
    std::vector<uint8_t> getSectionData(
        const std::string& sectionName) noexcept;
};

std::unique_ptr<MachOFileHandlerStrategy::Impl> make_macho_impl(
    const std::string& filename);

}  // namespace VMPilot::SDK::Segmentator

#endif  // __SDK_MACHO_HANDLER_HPP__