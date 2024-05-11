#ifndef __SDK_ELF_HANDLER_HPP__
#define __SDK_ELF_HANDLER_HPP__
#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace VMPilot::SDK::Segmentator {

class ELFHandler {
   public:
    ELFHandler(const std::string& file_name);
    ~ELFHandler();

    std::pair<uint64_t, uint64_t> getBeginEndAddr() noexcept;
    std::vector<uint8_t> getTextSection() noexcept;
    uint64_t getTextBaseAddr() noexcept;

   private:
    struct Impl;
    std::unique_ptr<Impl> pImpl;

    friend std::unique_ptr<Impl> make_elf_impl(const std::string& file_name);

   private:
    /**
     * Retrieves the index of an entry in the ".dynsym" section based on its signature.
     *
     * @param signature The signature of the entry to search for.
     * @return The index of the entry if found, or -1 if not found.
     */
    uint64_t getEntryIndex(const std::string& signature) noexcept;

    /**
     * @brief Retrieves the index of an entry in the ".rela.plt" section based on the index of the entry in the ".dynsym" section.
     *        This is used to find the index of the ".plt" section.
     * 
     * @param dynsym_idx The index of the entry in the ".dynsym" section.
     * @return The index of the entry in the ".rela.plt" section if found, or -1 if not found.
     */
    uint64_t getRelapltIdx(uint64_t dynsym_idx) noexcept;

    /**
     * @brief Retrieves the address of an entry in the ".plt" section based on the index of the entry in the ".rela.plt" section.
     * 
     * @param relaplt_idx The index of the entry in the ".rela.plt" section.
     * @return The address of the entry in the ".plt" section if found, or -1 if not found.
     */
    uint64_t getPltAddr(uint64_t relaplt_idx) noexcept;

    /**
     * @brief Get the begin and end address of the VMPilot signatures.
     */
    virtual std::pair<uint64_t, uint64_t> doGetBeginEndAddr() noexcept;

    /**
     * @brief Get the entire chuck of the .text section.
     */
    virtual std::vector<uint8_t> doGetTextSection() noexcept;
};

std::unique_ptr<ELFHandler::Impl> make_elf_impl(const std::string& file_name);
}  // namespace VMPilot::SDK::Segmentator

#endif  // __SDK_ELF_HANDLER_HPP__