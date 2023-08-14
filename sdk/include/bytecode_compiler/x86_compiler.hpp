#ifndef __SDK_X86_COMPILER_HPP__
#define __SDK_X86_COMPILER_HPP__

#include <bytecode_compiler.hpp>

namespace VMPilot::SDK::BytecodeCompiler {

class X86Compiler : public CompilerBase {
   public:
    /**
         * @brief Compile a script into bytecode.
         * 
         * @param script The script to compile.
         * @return std::vector<uint8_t> The compiled bytecode.
         */
    std::vector<uint8_t> Compile(const BytecodeCompileRecipe& script) override;

    /**
         * @brief Construct a new X86 Compiler object
         */
    X86Compiler() : CompilerBase("x86") {}
};

}  // namespace VMPilot::SDK::BytecodeCompiler

#endif  // __SDK_X86_COMPILER_HPP__