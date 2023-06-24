#ifndef __SDK_MACHO_SEG_HPP__
#define __SDK_MACHO_SEG_HPP__

#include <string>

namespace VMPilot::SDK::Segmentator {
/**
 * @brief A callable class that segments Mach-O files
 * 
 */
class MachO_Segmentator {
    std::string m_filename;

   public:
    /**
    * @brief Construct a new MachO_Segmentator object
    * 
    */
    MachO_Segmentator(const std::string& filename) : m_filename(filename) {}
    ~MachO_Segmentator() = default;

    /**
     * @brief Segments the Mach-O file
     * 
     */
    void operator()() noexcept;
};
};  // namespace VMPilot::SDK::Segmentator

#endif