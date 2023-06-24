#ifndef __SDK_ELF_SEG_HPP__
#define __SDK_ELF_SEG_HPP__

#include <string>

namespace VMPilot::SDK::Segmentator {
/**
 * @brief A callable class that segments ELF files
 * 
 */
class ELF_Segmentator {
    std::string m_filename;

   public:
    /**
     * @brief Construct a new ELF_Segmentator object
     * 
     */
    ELF_Segmentator(const std::string& filename) : m_filename(filename) {}
    ~ELF_Segmentator() = default;

    /**
     * @brief Segments the ELF file
     * 
     */
    void operator()() noexcept;
};
};  // namespace VMPilot::SDK::Segmentator

#endif