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

enum class SymbolSource {
    DYNAMIC,  // Dynamic symbol
    STATIC    // Static symbol
};

enum class SymbolVisibility {
    DEFAULT,   // Default visibility
    HIDDEN,    // Hidden visibility
    PROTECTED  // Protected visibility
};

enum class SymbolBinding {
    LOCAL,        // Local binding
    GLOBAL,       // Global binding
    WEAK,         // Weak binding
    LOOS = 10,    // OS-specific low
    HIOS = 12,    // OS-specific high
    LOPROC = 13,  // Processor-specific low
    HIPROC = 15   // Processor-specific high
};

// Structure to represent a symbol table entry
struct NativeSymbolTableEntry {
    std::string name;             // Symbol name
    uint64_t address;             // Address of the symbol
    uint64_t size;                // Size of the symbol
    SymbolSource source;          // Source of the symbol
    SymbolType type;              // Type of the symbol
    SymbolVisibility visibility;  // Visibility of the symbol
    SymbolBinding binding;        // Binding of the symbol
    uint64_t sectionNumber;       // Section number of the symbol
    uint64_t sectionIndex;        // Section index of the symbol

    std::unordered_map<std::string, std::variant<int, std::string, uint64_t>>
        additionalAttributes;

    NativeSymbolTableEntry()
        : name(""),
          address(0),
          size(0),
          source(SymbolSource::STATIC),
          type(SymbolType::NOTYPE),
          visibility(SymbolVisibility::DEFAULT),
          binding(SymbolBinding::LOCAL),
          sectionNumber(0),
          sectionIndex(0) {}

    NativeSymbolTableEntry(const std::string& name_, uint64_t address_,
                           uint64_t size_, SymbolSource source_,
                           SymbolType type_, SymbolVisibility visibility_,
                           SymbolBinding binding_, uint64_t sectionNumber_,
                           uint64_t sectionIndex_)
        : name(name_),
          address(address_),
          size(size_),
          source(source_),
          type(type_),
          visibility(visibility_),
          binding(binding_),
          sectionNumber(sectionNumber_),
          sectionIndex(sectionIndex_) {}

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