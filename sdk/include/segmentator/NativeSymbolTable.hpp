#ifndef __SDK_SEGMENTATOR_NATIVE_SYMBOL_TABLE_HPP__
#define __SDK_SEGMENTATOR_NATIVE_SYMBOL_TABLE_HPP__
#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

namespace VMPilot::SDK::Segmentator {
// A symbol table entry class for all architectures
enum class SymbolType {
    NOTYPE,          // No type specified
    OBJECT,          // Data object
    FUNC,            // Function or executable code
    SECTION,         // Section symbol
    FILE,            // File symbol
    LOPROC = 13,     // Processor-specific low
    HIPROC = 15,     // Processor-specific high
    SPARC_REGISTER,  // SPARC-specific register
    // Extend with additional COFF-specific types if necessary
};

// Structure to represent a symbol table entry
struct NativeSymbolTableEntry {
    std::string name;  // Symbol name
    uint64_t address;  // Address of the symbol
    uint64_t size;     // Size of the symbol
    SymbolType type;   // Type of the symbol
    bool isGlobal;     // Visibility - global or local

    std::unordered_map<std::string, std::variant<int, std::string, uint64_t>>
        additionalAttributes;

    NativeSymbolTableEntry()
        : name(""),
          address(0),
          size(0),
          type(SymbolType::NOTYPE),
          isGlobal(false) {}

    void setAttribute(const std::string& key,
                      const std::variant<int, std::string, uint64_t>& value) {
        additionalAttributes[key] = value;
    }

    template <typename T>
    T getAttribute(const std::string& key, const T& defaultValue) const {
        auto it = additionalAttributes.find(key);
        if (it != additionalAttributes.end()) {
            try {
                return std::get<T>(it->second);
            } catch (const std::bad_variant_access&) {
                // Handle or log the type mismatch issue appropriately
            }
        }
        return defaultValue;
    }

    // Convenience functions for common PE attributes
    void setSectionNumber(int sectionNumber) {
        setAttribute("section_number", sectionNumber);
    }

    int getSectionNumber() const {
        return getAttribute<int>("section_number",
                                 -1);  // -1 indicates an undefined section
    }

    void setStorageClass(int storageClass) {
        setAttribute("storage_class", storageClass);
    }

    int getStorageClass() const {
        return getAttribute<int>("storage_class",
                                 0);  // 0 as default (undefined)
    }

    void setNumberOfAuxSymbols(int auxCount) {
        setAttribute("aux_symbols", auxCount);
    }

    int getNumberOfAuxSymbols() const {
        return getAttribute<int>("aux_symbols",
                                 0);  // No auxiliary symbols by default
    }
};

using NativeSymbolTable = std::vector<NativeSymbolTableEntry>;

}  // namespace VMPilot::SDK::Segmentator

#endif  // __SDK_SEGMENTATOR_NATIVE_SYMBOL_TABLE_HPP__