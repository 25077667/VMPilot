#ifndef __SDK_INSN_SEG_HPP__
#define __SDK_INSN_SEG_HPP__
#pragma once

#include <ArchEnum.hpp>
#include <ModeEnum.hpp>

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace VMPilot::SDK::Segmentator {

// Use Pimpl to hold a capstone object
class InsnSegmentator {
   public:
    InsnSegmentator(Arch arch, Mode mode);
    ~InsnSegmentator();
    bool load(const std::vector<uint8_t>& code,
              const uint64_t base_addr) noexcept;

   protected:
    struct Impl;
    std::unique_ptr<Impl> pImpl;

   protected:
    virtual bool doLoad(const std::vector<uint8_t>& code,
                        const uint64_t base_addr) noexcept;
};

}  // namespace VMPilot::SDK::Segmentator

#endif  // __SDK_INSN_SEG_HPP__