#ifndef __SDK_BYTECODE_COMPILER_HPP__
#define __SDK_BYTECODE_COMPILER_HPP__

/**
 * @brief A factory class for creating bytecode compilers.
 */

#include <BytecodeCompileRecipe.hpp>

#include <memory>
#include <string>
#include <vector>

namespace VMPilot::SDK::BytecodeCompiler {
// Defined the base class for a bytecode compiler.
class CompilerBase {
    std::string name;

   public:
    /**
     * @brief Compile a script into bytecode.
     * 
     * @param script The script to compile.
     * @return std::vector<uint8_t> The compiled bytecode.
     */
    virtual std::vector<uint8_t> Compile(const BytecodeCompileRecipe& script) = 0;

    /**
     * @brief Get the name of the compiler.
     *  This is used to identify the compiler in the configuration file.
     */
    inline std::string GetName() const noexcept { return name; }

    /**
     * @brief Construct a new Compiler Base object
     * 
     * @param name The name of the compiler.
     */
    CompilerBase(std::string name) : name(name) {}

    /**
     * @brief Destroy the Compiler Base object
     */
    virtual ~CompilerBase() = default;
};

// Define the factory class for creating bytecode compilers.
class CompilerFactory {
   public:
    /**
     * @brief Create a new compiler.
     * 
     * @param name The name of the compiler to create.
     * @return std::unique_ptr<CompilerBase> The created compiler.
     */
    static std::unique_ptr<CompilerBase> CreateCompiler(
        const std::string& name) noexcept;
};

class _NotImplementedYet : public CompilerBase {
   public:
    /**
     * @brief Compile a script into bytecode.
     * 
     * @param script The script to compile.
     * @return std::vector<uint8_t> The compiled bytecode.
     */
    std::vector<uint8_t> Compile(const BytecodeCompileRecipe& script) override;

    /**
     * @brief Construct a new _NotImplementedYet object
     */
    _NotImplementedYet() : CompilerBase("not_implemented") {}

    /**
     * @brief Construct a new _NotImplementedYet object
     * 
     * @param name The name of the compiler.
     */
    _NotImplementedYet(std::string name) : CompilerBase(name) {}
};

}  // namespace VMPilot::SDK::BytecodeCompiler

#endif