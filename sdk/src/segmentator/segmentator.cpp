#include <elf_seg.hpp>
#include <file_type_parser.hpp>
#include <macho_seg.hpp>
#include <pe_seg.hpp>
#include <segmentator.hpp>

#include <iostream>

using namespace VMPilot::SDK::Segmentator;

namespace detail {
std::string get_file_type(const std::string& filename);
};

Segmentator::Segmentator(const std::string& filename) : m_filename(filename) {
    const auto fileType = detail::get_file_type(filename);
    std::cout << "File type: " << fileType << std::endl;

    if (fileType == "ELF") {
        m_callback = [](const std::string& filename) {
            auto me = ELF_Segmentator(filename);
            me();
        };
    } else if (fileType == "Mach-O") {
        m_callback = [](const std::string& filename) {
            auto me = MachO_Segmentator(filename);
            me();
        };
    } else if (fileType == "PE") {
        m_callback = [](const std::string& filename) {
            auto me = PE_Segmentator(filename);
            me();
        };
    } else {
        std::cerr << "Error: Unknown file type" << std::endl;
    }
}
void Segmentator::do_segmentation() noexcept {
    m_callback(m_filename);
}

std::string detail::get_file_type(const std::string& filename) {
    try {
        // Get the file type
        const auto& fileTypeParser = VMPilot::Common::FileTypeParser(filename);
        return fileTypeParser.GetFileType();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return "Unknown";
    }
}