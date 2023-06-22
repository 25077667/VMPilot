#ifndef __SDK_X86_64_COMPILER_HPP__
#define __SDK_X86_64_COMPILER_HPP__

#include <bytecode_compiler.hpp>

namespace VMPilot::SDK::BytecodeCompiler {

class X86_64Compiler : public _NotImplementedYet {
   public:
    /**
         * @brief Compile a script into bytecode.
         * 
         * @param script The script to compile.
         * @return std::vector<uint8_t> The compiled bytecode.
         */
    std::vector<uint8_t> Compile(const ScriptRecipe& script) override;

    /**
         * @brief Construct a new X86_64 Compiler object
         */
    X86_64Compiler() : _NotImplementedYet("x86_64") {}
};

};  // namespace VMPilot::SDK::BytecodeCompiler

#endif  // __SDK_X86_64_COMPILER_HPP__