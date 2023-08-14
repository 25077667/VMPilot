#include <BytecodeCompileRecipe.hpp>
#include <string>

using namespace VMPilot::SDK;

namespace detail {
// We throw all exceptions here:
void check_master_key(const std::string& key);
}  // namespace detail

bool BytecodeCompileRecipe::check() const {
    // Check the master key
    if (script.contains("master_key")) {
        detail::check_master_key(script["master_key"]);
    } else if (!script.contains("master_key")) {
        throw std::runtime_error("The master key is not set.");
    }

    // Check the input binary filename
    if (!script.contains("input")) {
        throw std::runtime_error("The input binary filename is not set.");
    }

    // Check the output binary filename
    if (!script.contains("output")) {
        throw std::runtime_error("The output binary filename is not set.");
    }

    return true;
}

void detail::check_master_key(const std::string& key) {
    // The key's length must longer than 32 bytes
    if (key.length() < 32) {
        auto msg = "The master key's length must longer than 32 bytes: " + key;
        throw std::runtime_error(msg);
    }
}