#include <cstdint>
#include <file_type_parser.hpp>
#include <fstream>
#include <vector>

using namespace VMPilot::Common;

namespace detail {
std::vector<uint8_t> get_file_header_block(const std::string filename);
};

FileTypeParser::FileTypeParser(const std::string& filename)
    : m_filename(filename) {
    // Read the file header and determine the file type
    // Assign the file type to m_fileType
    // Only supports ELF, PE, and Mach-O

    // Get the file header
    const auto& header = detail::get_file_header_block(filename);
    // Check the file type, use the magic number
    if (header[0] == 0x7f && header[1] == 'E' && header[2] == 'L' &&
        header[3] == 'F') {
        m_fileType = "ELF";
    } else if (header[0] == 'M' && header[1] == 'Z') {
        m_fileType = "PE";
    } else if (header[0] == 0xfe && header[1] == 0xed && header[2] == 0xfa &&
               header[3] == 0xce) {
        m_fileType = "Mach-O";
    } else {
        m_fileType = "Unknown";
    }

    if (m_fileType == "Unknown") {
        throw std::runtime_error(
            "Unknown file type, neither ELF, PE, nor Mach-O");
    }
}

std::vector<uint8_t> detail::get_file_header_block(const std::string filename) {
    std::ifstream file(filename, std::ios::binary);
    // check the file is exist or not
    if (!file) {
        // It might be file not found error
        // Or it might be permission denied error
        throw std::runtime_error("File not found or permission denied");
    }

    std::vector<uint8_t> header;
    if (file.is_open()) {
        // Read the first 8 bytes of the file
        header.resize(8);
        file.read(reinterpret_cast<char*>(header.data()), 8);
        file.close();
    }
    return header;
}

std::string FileTypeParser::GetFileType() const noexcept {
    return m_fileType;
}