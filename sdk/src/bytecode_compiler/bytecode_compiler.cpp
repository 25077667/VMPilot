#include <bytecode_compiler.hpp>
#include <x86_64_compiler.hpp>
#include <x86_compiler.hpp>

using namespace VMPilot::SDK::BytecodeCompiler;

std::unique_ptr<CompilerBase> CompilerFactory::CreateCompiler(
    const std::string& name) noexcept {
    if (name == "x86") {
        return std::make_unique<X86Compiler>();
    } else if (name == "x86_64") {
        return std::make_unique<X86_64Compiler>();
    } else if (name == "arm") {
        return std::make_unique<_NotImplementedYet>();
    } else if (name == "arm64") {
        return std::make_unique<_NotImplementedYet>();
    }
    return nullptr;
}

std::vector<uint8_t> _NotImplementedYet::Compile(
    [[maybe_unused]] const BytecodeCompileRecipe& script) {
    throw std::runtime_error(
        "Not implemented yet, but it is in the future work.");
}