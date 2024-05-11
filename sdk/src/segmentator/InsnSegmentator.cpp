#include <InsnSegmentator.hpp>

#include <capstone.hpp>

using namespace VMPilot::SDK::Segmentator;

struct InsnSegmentator::Impl {
    Capstone::Capstone cs;
    std::vector<Capstone::Instruction> instructions;
    uint64_t base_addr;
};

InsnSegmentator::InsnSegmentator(Arch arch, Mode mode)
    : pImpl{new Impl{Capstone::Capstone(static_cast<Capstone::Arch>(arch),
                                        static_cast<Capstone::Mode>(mode))}} {}

InsnSegmentator::~InsnSegmentator() = default;

bool InsnSegmentator::load(const std::vector<uint8_t>& code,
                           const uint64_t base_addr) noexcept {
    return doLoad(code, base_addr);
}

bool InsnSegmentator::doLoad(const std::vector<uint8_t>& code,
                             const uint64_t base_addr) noexcept {
    auto& impl = this->pImpl;
    impl->base_addr = base_addr;
    impl->instructions = impl->cs.disasm(code);
    return !impl->instructions.empty();
}
