#ifndef __SDK_NATIVE_FUNCTION_BASE_HPP__
#define __SDK_NATIVE_FUNCTION_BASE_HPP__
#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace VMPilot::SDK::Segmentator {
/**
 * @brief Base class for native functions in different executable formats.
 * 
 * This class is intended to be inherited by derived classes for different
 * executable formats such as ELF, PE, etc.
 */
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
}  // namespace VMPilot::SDK::Segmentator

#endif  // __SDK_NATIVE_FUNCTION_BASE_HPP__