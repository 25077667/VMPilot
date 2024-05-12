#ifndef __SDK_SEGMENTATOR_STRATEGY_HPP__
#define __SDK_SEGMENTATOR_STRATEGY_HPP__
#pragma once

#include <ArchEnum.hpp>
#include <ModeEnum.hpp>

#include <cstdint>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace VMPilot::SDK::Segmentator {

// Which would be inherited by the derived class for ELF, PE, and so on
class NativeFunctionBase {
   protected:
    uint64_t m_addr;
    uint64_t m_size;
    std::string m_name;
    std::vector<uint8_t> m_code;
    std::vector<uint8_t> globalData;
    friend class ArchHandlerStrategy;

   public:
    NativeFunctionBase(uint64_t addr, uint64_t size, std::string name,
                       std::vector<uint8_t> code)
        : m_addr(addr), m_size(size), m_name(name), m_code(code) {}

    virtual ~NativeFunctionBase() = default;
    uint64_t getAddr() const { return m_addr; }
    uint64_t getSize() const { return m_size; }
    std::string getName() const { return m_name; }
    std::vector<uint8_t> getCode() const { return m_code; }
    std::vector<uint8_t> getGlobalData() const { return globalData; }
};

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
    Arch m_arch;
    Mode m_mode;

   protected:
    /**
    * @brief Load text code to derived pimpl class
    * 
    * @param code The code to load
    * @param base_addr The base address of the code
    * @return true if the code is loaded successfully, false otherwise
    */
    virtual bool doLoad(const std::vector<uint8_t>& code,
                        const uint64_t base_addr);

    /**
     * @brief Get the native functions from the derived pimpl class
     * 
     * @return std::vector<std::unique_ptr<NativeFunctionBase>> The native functions
     */
    virtual std::vector<std::unique_ptr<NativeFunctionBase>>
    doGetNativeFunctions();

   public:
    virtual ~ArchHandlerStrategy() = default;
    ArchHandlerStrategy() : ArchHandlerStrategy(Arch::X86, Mode::MODE_64) {}
    ArchHandlerStrategy(Arch arch, Mode mode) : m_arch(arch), m_mode(mode) {}

    /**
     * @brief Load text code to derived pimpl class
     * 
     * @param code The code to load
     * @param base_addr The base address of the code
     * @return true if the code is loaded successfully, false otherwise
     */
    bool Load(const std::vector<uint8_t>& code, const uint64_t base_addr) {
        return doLoad(code, base_addr);
    }

    /**
     * @brief Get the native functions from the derived pimpl class
     * 
     * @return std::vector<std::unique_ptr<NativeFunctionBase>> The native functions
     */
    std::vector<std::unique_ptr<NativeFunctionBase>> getNativeFunctions() {
        return doGetNativeFunctions();
    }
};

}  // namespace VMPilot::SDK::Segmentator

#endif  // __SDK_SEGMENTATOR_STRATEGY_HPP__