#ifndef __SDK_ELF_SEG_HPP__
#define __SDK_ELF_SEG_HPP__
#include <segmentator/segmentator.hpp>

#include <string>

namespace VMPilot::SDK::Segmentator {
/**
 * @brief A callable class that segments ELF files
 * 
 */
class ELF_Segmentator : public Segmentator {
   protected:
    virtual void segmentation() noexcept override;

   public:
    /**
     * @brief Construct a new ELF_Segmentator object
     */
    ELF_Segmentator(const std::string& filename)
        : Segmentator(filename, "ELF") {}
    ~ELF_Segmentator() = default;
};
};  // namespace VMPilot::SDK::Segmentator

#endif