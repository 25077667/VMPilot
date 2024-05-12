#include <X86Handler.hpp>

#include <algorithm>

#include <capstone.hpp>

using namespace VMPilot::SDK::Segmentator;

struct X86Handler::Impl {
    Capstone::Capstone cs;
    uint64_t base_addr = -1;
    std::vector<Capstone::Instruction> instructions;
    std::vector<std::unique_ptr<NativeFunctionBase>> native_functions;

    Impl(Capstone::Capstone&& cs) : cs(std::move(cs)) {}
};

X86Handler::X86Handler(Mode mode)
    : ArchHandlerStrategy(Arch::X86, mode),
      pImpl(make_x86_handler_impl(mode)) {}

std::unique_ptr<X86Handler::Impl>
VMPilot::SDK::Segmentator ::make_x86_handler_impl(Mode mode) {
    return std::make_unique<X86Handler::Impl>(Capstone::Capstone(
        Capstone::Arch::X86, static_cast<Capstone::Mode>(mode)));
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
    auto& native_functions = this->pImpl->native_functions;
    if (native_functions.empty()) {
        // TODO: Implement this function
    }

    // copy to avoid move
    std::vector<std::unique_ptr<NativeFunctionBase>> result;
    std::for_each(
        native_functions.begin(), native_functions.end(),
        [&result](const auto& nf) {
            result.push_back(std::make_unique<NativeFunctionBase>(
                nf->getAddr(), nf->getSize(), nf->getName(), nf->getCode()));
        });
    return result;
}
