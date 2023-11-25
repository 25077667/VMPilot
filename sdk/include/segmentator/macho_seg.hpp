#ifndef __SDK_MACHO_SEG_HPP__
#define __SDK_MACHO_SEG_HPP__
#include <segmentator/segmentator.hpp>

#include <string>

namespace VMPilot::SDK::Segmentator {
/**
 * @brief A callable class that segments Mach-O files
 * 
 */
class MachO_Segmentator : public Segmentator {
   protected:
    virtual void segmentation() noexcept override;

   public:
    /**
    * @brief Construct a new MachO_Segmentator object
    */
    MachO_Segmentator(const std::string& filename)
        : Segmentator(filename, "Mach-O") {}
    ~MachO_Segmentator() = default;
};
};  // namespace VMPilot::SDK::Segmentator

#endif