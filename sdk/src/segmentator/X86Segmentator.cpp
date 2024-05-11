#include <X86Segmentator.hpp>

using namespace VMPilot::SDK::Segmentator;

bool X86Segmentator::doLoad(const std::vector<uint8_t>& code,
                            const uint64_t base_addr) noexcept {
    auto& impl = this->pImpl;

    impl->base_addr = base_addr;
    impl->instructions = impl->cs.disasm(code);
    return !impl->instructions.empty();
}
