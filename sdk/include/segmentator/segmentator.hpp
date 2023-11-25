#ifndef __SDK_SEGMENTATOR_HPP__
#define __SDK_SEGMENTATOR_HPP__

#include <functional>
#include <memory>
#include <string>

namespace VMPilot::SDK::Segmentator {
class Segmentator {
   protected:
    std::string m_filename;
    std::string m_filetype;

    virtual void segmentation() noexcept = 0;

   public:
    Segmentator(const std::string& filename)
        : m_filename(filename), m_filetype("unknown") {}
    Segmentator(const std::string& filename, const std::string& filetype)
        : m_filename(filename), m_filetype(filetype) {}

    ~Segmentator() = default;

    void operator()() noexcept { segmentation(); }
    std::string get_filetype() const noexcept { return m_filetype; }
};

std::unique_ptr<Segmentator> create_segmentator(
    const std::string& filename) noexcept;

}  // namespace VMPilot::SDK::Segmentator

#endif