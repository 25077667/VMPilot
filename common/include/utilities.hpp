#ifndef __COMMON_UTILITIES_HPP__
#define __COMMON_UTILITIES_HPP__

#include <cstdint>
#include <string>
#include <vector>

namespace VMPilot::Common {
extern const std::string BEGIN_VMPILOT_SIGNATURE;
extern const std::string END_VMPILOT_SIGNATURE;

std::vector<uint8_t> read_file(const std::string& path) noexcept;
}  // namespace VMPilot::Common

#endif