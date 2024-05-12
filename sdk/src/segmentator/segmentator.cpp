#include <segmentator.hpp>

#include <ELFHandler.hpp>
#include <X86Handler.hpp>
#include <file_type_parser.hpp>

#include <spdlog/spdlog.h>

using namespace VMPilot::SDK::Segmentator;

namespace {
namespace detail {
static const std::unordered_map<
    VMPilot::Common::FileFormat,
    std::function<std::unique_ptr<FileHandlerStrategy>(const std::string&)>>
    file_strategy_table = {
        {VMPilot::Common::FileFormat::ELF,
         [](const std::string& filename) {
             return std::make_unique<ELFFileHandlerStrategy>(filename);
         }},
};

// architecture strategy table, ArchHandlerStrategy
static const std::unordered_map<
    VMPilot::Common::FileArch,
    std::function<std::unique_ptr<ArchHandlerStrategy>(
        VMPilot::Common::FileMode)>>
    arch_strategy_table = {
        {VMPilot::Common::FileArch::X86,
         [](VMPilot::Common::FileMode mode) {
             return std::make_unique<X86Handler>(mode);
         }},
};

}  // namespace detail
}  // namespace

std::unique_ptr<Segmentator> VMPilot::SDK::Segmentator::create_segmentator(
    const std::string& filename) noexcept {
    auto segmentator = std::unique_ptr<Segmentator>(nullptr);

    try {
        segmentator->m_metadata = VMPilot::Common::get_file_metadata(filename);
    } catch (const std::exception& e) {
        spdlog::error("Error creating segmentator: {}", e.what());
    }

    const auto& format = segmentator->m_metadata.format;
    auto it = detail::file_strategy_table.find(format);
    if (it != detail::file_strategy_table.end()) {
        segmentator->m_file_handler = it->second(filename);
    } else {
        spdlog::error("Unsupported file format: {}",
                      static_cast<uint8_t>(format));
    }

    const auto& arch = segmentator->m_metadata.arch;
    auto it2 = detail::arch_strategy_table.find(arch);
    if (it2 != detail::arch_strategy_table.end()) {
        segmentator->m_arch_handler = it2->second(segmentator->m_metadata.mode);
    } else {
        spdlog::error("Unsupported architecture: {}",
                      static_cast<uint8_t>(arch));
    }

    return segmentator;
}
