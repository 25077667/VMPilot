#ifndef __COMMON_FILE_TYPE_PARSER_HPP__
#define __COMMON_FILE_TYPE_PARSER_HPP__

#include <string>

namespace VMPilot::Common {
class FileTypeParser {
   private:
    std::string m_filename;
    std::string m_fileType;

   public:
    /**
     * @brief Construct a new File Type Parser object
     * 
     * @param filename The file name
     * 
     * It would throw exceptions if the file is not found, permission denied
     * or unknown file type
     */
    FileTypeParser(const std::string& filename);
    ~FileTypeParser() = default;

    /**
     * @brief Get the File Type object
     * 
     * @return std::string The file type
     */
    std::string GetFileType() const noexcept;
};
};  // namespace VMPilot::Common

#endif  // __COMMON_FILE_TYPE_PARSER_HPP__