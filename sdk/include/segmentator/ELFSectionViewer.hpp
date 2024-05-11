#ifndef __ELF_SECTION_VIEWER_HPP__
#define __ELF_SECTION_VIEWER_HPP__
#pragma once

#include <elfio/elf_types.hpp>
#include <elfio/elfio.hpp>

#include <memory>
#include <string>
#include <vector>

namespace VMPilot::SDK::Segmentator {

/**
 * @brief A viewer class for the ELFIO::section class.
 * 
 * we not take ownership of the section object.
 * This class is used to provide a more convenient way to access the section object.
 */
class ELFSectionViewer {
   public:
    ELFSectionViewer() = default;
    ELFSectionViewer(ELFIO::section* sec) : section(sec) {}

    ELFSectionViewer(const ELFSectionViewer&) = delete;
    ELFSectionViewer& operator=(const ELFSectionViewer&) = delete;
    ELFSectionViewer(ELFSectionViewer&&) = default;
    ELFSectionViewer& operator=(ELFSectionViewer&&) = default;
    ~ELFSectionViewer() = default;

    ELFIO::section* getSection() noexcept { return section; }

   private:
    ELFIO::section* section = nullptr;
};

}  // namespace VMPilot::SDK::Segmentator

#endif  // __ELF_SECTION_VIEWER_HPP__