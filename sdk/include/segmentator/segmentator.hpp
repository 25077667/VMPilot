#ifndef __SDK_SEGMENTATOR_HPP__
#define __SDK_SEGMENTATOR_HPP__

#include <functional>
#include <string>

namespace VMPilot::SDK::Segmentator {
class Segmentator {
    std::string m_filename;
    std::function<void(std::string)> m_callback;

   public:
    Segmentator(const std::string& filename);
    ~Segmentator() = default;

    void do_segmentation() noexcept;
};
}  // namespace VMPilot::SDK::Segmentator

#endif