#include <ELFHandler.hpp>
#include <elf_seg.hpp>
#include <utilities.hpp>

#include <cstdint>
#include <fstream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <spdlog/spdlog.h>

void VMPilot::SDK::Segmentator::ELF_Segmentator::segmentation() noexcept {
    try {
        ELFFileHandlerStrategy elfHandler(m_filename);

        auto addr_pair = elfHandler.getBeginEndAddr();
        this->m_begin_addr = addr_pair.first;
        this->m_end_addr = addr_pair.second;

        auto text_section = elfHandler.getTextSection();
        this->m_code.swap(text_section);

        this->m_base_addr = elfHandler.getTextBaseAddr();
    } catch (const std::exception& e) {
        spdlog::error("Error segmenting ELF file: {}", e.what());
    }
}