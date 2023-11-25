#include <elf_seg.hpp>
#include <file_type_parser.hpp>
#include <macho_seg.hpp>
#include <pe_seg.hpp>
#include <segmentator.hpp>

#include <spdlog/spdlog.h>

using namespace VMPilot::SDK::Segmentator;

namespace detail {
std::string get_file_type(const std::string& filename) noexcept;
};  // namespace detail

std::string detail::get_file_type(const std::string& filename) noexcept {
    try {
        // Get the file type
        const auto& fileTypeParser = VMPilot::Common::FileTypeParser(filename);
        return fileTypeParser.GetFileType();
    } catch (const std::exception& e) {
        spdlog::error("Error getting file type: {}", e.what());
        return "Unknown";
    }
}

std::unique_ptr<Segmentator> VMPilot::SDK::Segmentator::create_segmentator(
    const std::string& filename) noexcept {
    const auto& fileType = detail::get_file_type(filename);
    if (fileType == "ELF")
        return std::make_unique<ELF_Segmentator>(filename);
    else if (fileType == "PE")
        return std::make_unique<PE_Segmentator>(filename);
    else if (fileType == "Mach-O")
        return std::make_unique<MachO_Segmentator>(filename);
    else
        return nullptr;
}
