#ifndef __SDK_SEGMENTATOR_HPP__
#define __SDK_SEGMENTATOR_HPP__

#include <Strategy.hpp>
#include <file_type_parser.hpp>

#include <cstdint>
#include <functional>
#include <memory>
#include <string>

namespace VMPilot::SDK::Segmentator {
class Segmentator {
   protected:
    VMPilot::Common::FileMetadata m_metadata;
    std::unique_ptr<FileHandlerStrategy> m_file_handler;
    std::unique_ptr<ArchHandlerStrategy> m_arch_handler;

    friend std::unique_ptr<Segmentator> create_segmentator(
        const std::string& filename) noexcept;

   protected:
    virtual void segmentation() noexcept;

   public:
    // not allowed to default construct, please use create_segmentator
    Segmentator() = delete;
    virtual ~Segmentator() = default;
};

std::unique_ptr<Segmentator> create_segmentator(
    const std::string& filename) noexcept;

}  // namespace VMPilot::SDK::Segmentator

#endif