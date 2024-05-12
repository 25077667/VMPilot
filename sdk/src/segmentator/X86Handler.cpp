#include <X86Handler.hpp>

#include <capstone.hpp>

using namespace VMPilot::SDK::Segmentator;

struct X86Handler::Impl {
    Capstone::Capstone cs;
    std::vector<Capstone::Instruction> instructions;
    uint64_t base_addr = -1;

    Impl(Capstone::Capstone&& cs,
         std::vector<Capstone::Instruction>&& instructions,
         const uint64_t base_addr)
        : cs(std::move(cs)),
          instructions(std::move(instructions)),
          base_addr(base_addr) {}
};

X86Handler::X86Handler(Mode mode)
    : ArchHandlerStrategy(Arch::X86, mode),
      pImpl(make_x86_handler_impl(mode)) {}

std::unique_ptr<X86Handler::Impl>
VMPilot::SDK::Segmentator ::make_x86_handler_impl(Mode mode) {
    return std::make_unique<X86Handler::Impl>(
        Capstone::Capstone(Capstone::Arch::X86,
                           static_cast<Capstone::Mode>(mode)),
        std::vector<Capstone::Instruction>(), -1);
}

X86Handler::~X86Handler() = default;

bool X86Handler::doLoad(const std::vector<uint8_t>& code,
                        const uint64_t base_addr) noexcept {
    auto& impl = this->pImpl;

    impl->base_addr = base_addr;
    impl->instructions = impl->cs.disasm(code);
    return !impl->instructions.empty();
}

std::vector<std::unique_ptr<NativeFunctionBase>>
X86Handler::doGetNativeFunctions() noexcept {
    std::vector<std::unique_ptr<NativeFunctionBase>> native_functions;

    // TODO: Implement this function

    return native_functions;
}
