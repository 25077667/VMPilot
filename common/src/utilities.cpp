#include <utilities.hpp>

#include <fstream>

const std::string VMPilot::Common::BEGIN_VMPILOT_SIGNATURE =
    []() noexcept -> std::string {
// Mangling of VMPilot_Begin(__FUNCTION__); signature
#ifdef _WIN64
    return "?VMPilot_Begin@@YAXPEBD@Z";
#elif _WIN32
    return "?VMPilot_Begin@@YAXPBD@Z";
#else  // gcc or clang
    return "_Z13VMPilot_BeginPKc";
#endif
}();

const std::string VMPilot::Common::END_VMPILOT_SIGNATURE =
    []() noexcept -> std::string {
// Mangling of VMPilot_End(__FUNCTION__); signature
#ifdef _WIN64
    return "?VMPilot_End@@YAXPEBD@Z";
#elif _WIN32
    return "?VMPilot_End@@YAXPBD@Z";
#else  // gcc or clang
    return "_Z11VMPilot_EndPKc";
#endif
}();

std::vector<uint8_t> VMPilot::Common::read_file(
    const std::string& filename) noexcept {
    auto f = std::ifstream(filename, std::ios::binary);
    if (!f.is_open())
        return {};

    f.seekg(0, std::ios::end);
    auto size = f.tellg();
    f.seekg(0, std::ios::beg);

    std::vector<uint8_t> buffer = std::vector<uint8_t>(size);
    f.read(reinterpret_cast<char*>(buffer.data()), size);
    f.close();

    return buffer;
}