#ifndef __SDK_SCRIPT_RECIPE_HPP__
#define __SDK_SCRIPT_RECIPE_HPP__

#include <nlohmann/json.hpp>

namespace VMPilot::SDK {
class ScriptRecipe {
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
    ScriptRecipe(nlohmann::json script) : script(script) { check(); }
};
};  // namespace VMPilot::SDK

#endif  // __SDK_SCRIPT_RECIPE_HPP__