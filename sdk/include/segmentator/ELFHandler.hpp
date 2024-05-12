#ifndef __SDK_ELF_HANDLER_HPP__
#define __SDK_ELF_HANDLER_HPP__
#pragma once

#include <Strategy.hpp>

#include <cstdint>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace VMPilot::SDK::Segmentator {

class ELFFileHandlerStrategy : public FileHandlerStrategy {
   private:
    struct Impl;
    std::unique_ptr<Impl> pImpl;
    // Private method to create an instance of the implementation
    friend std::unique_ptr<Impl> make_elf_impl(const std::string& filename);

   public:
    explicit ELFFileHandlerStrategy(const std::string& filename);
    virtual ~ELFFileHandlerStrategy();

   protected:
    /**
     * @brief Get the begin and end address of the VMPilot signatures.
     */
    virtual std::pair<uint64_t, uint64_t> doGetBeginEndAddr() noexcept override;

    /**
     * @brief Get the entire chuck of the .text section.
     */
    virtual std::vector<uint8_t> doGetTextSection() noexcept override;

    /**
     * @brief Get the base address of the .text section.
     */
    virtual uint64_t doGetTextBaseAddr() noexcept override;

    /**
     * @brief Get the native symbol table.
     */
    virtual NativeSymbolTable doGetNativeSymbolTable() noexcept;

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
     * @brief Internal implementation of doGetBeginEndAddr.
     *        This method is used to perform the actual logic of getting the begin and end address of the VMPilot signatures.
     *        The caching logic is left in the doGetBeginEndAddr method.
     * 
     * @return A pair of uint64_t values representing the begin and end addresses.
     */
    std::pair<uint64_t, uint64_t> doGetBeginEndAddrIntl() noexcept;

    /**
     * @brief Internal implementation of doGetTextSection.
     *        This method is used to perform the actual logic of getting the entire chuck of the .text section.
     *        The caching logic is left in the doGetTextSection method.
     * 
     * @return A vector of uint8_t values representing the entire chuck of the .text section.
     */
    std::vector<uint8_t> doGetTextSectionIntl() noexcept;
};

std::unique_ptr<ELFFileHandlerStrategy::Impl> make_elf_impl(
    const std::string& filename);

}  // namespace VMPilot::SDK::Segmentator

#endif  // __SDK_ELF_HANDLER_HPP__