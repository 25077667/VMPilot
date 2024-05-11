#ifndef __SDK_X86_SEGMENTATOR_HPP__
#define __SDK_X86_SEGMENTATOR_HPP__
#pragma once

#include <InsnSegmentator.hpp>

namespace VMPilot::SDK::Segmentator {
class X86Segmentator : public InsnSegmentator {
   public:
    X86Segmentator(Arch arch, Mode mode) : InsnSegmentator(arch, mode) {}
    virtual ~X86Segmentator() = default;

   protected:
    virtual bool doLoad(const std::vector<uint8_t>& code,
                        const uint64_t base_addr) noexcept override;
};
}  // namespace VMPilot::SDK::Segmentator

#endif  // __SDK_X86_SEGMENTATOR_HPP__