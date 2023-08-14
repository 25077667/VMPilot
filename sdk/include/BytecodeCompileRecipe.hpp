#ifndef __SDK_BytecodeCompileRecipe_HPP__
#define __SDK_BytecodeCompileRecipe_HPP__

#include <nlohmann/json.hpp>

namespace VMPilot::SDK {
class BytecodeCompileRecipe {
    nlohmann::json script;
    /**
     * @brief Check if the script is valid.
     * 
     * @return true If the script is valid.
     * 
     * It will throw an exception if the script is invalid.
     * Must be called before the script is compiled.
     */
    bool check() const;

   public:
    /**
     * @brief Construct a new Script Recipe object
     * 
     * @param script The script to compile.
     */
    BytecodeCompileRecipe(nlohmann::json script) : script(script) { check(); }
};
}  // namespace VMPilot::SDK

#endif  // __SDK_BytecodeCompileRecipe_HPP__