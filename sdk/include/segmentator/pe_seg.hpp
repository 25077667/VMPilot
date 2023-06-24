#ifndef __SDK_PE_SEG_HPP__
#define __SDK_PE_SEG_HPP__

#include <string>

namespace VMPilot::SDK::Segmentator {
/**
 * @brief A callable class that segments PE files
 * 
 */
class PE_Segmentator {
    std::string m_filename;

   public:
    /**
     * @brief Construct a new PE_Segmentator object
     * 
     */
    PE_Segmentator(const std::string& filename) : m_filename(filename) {}
    ~PE_Segmentator() = default;

    /**
     * @brief Segments the PE file
     * 
     */
    void operator()() noexcept;
};
};  // namespace VMPilot::SDK::Segmentator
#endif