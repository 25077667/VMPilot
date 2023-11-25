#ifndef __SDK_PE_SEG_HPP__
#define __SDK_PE_SEG_HPP__
#include <segmentator/segmentator.hpp>

#include <string>

namespace VMPilot::SDK::Segmentator {
/**
 * @brief A callable class that segments PE files
 * 
 */
class PE_Segmentator : public Segmentator {
   protected:
    virtual void segmentation() noexcept override;

   public:
    /**
     * @brief Construct a new PE_Segmentator object
     */
    PE_Segmentator(const std::string& filename) : Segmentator(filename, "PE") {}
    ~PE_Segmentator() = default;
};
};  // namespace VMPilot::SDK::Segmentator
#endif