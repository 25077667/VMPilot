#ifndef __RUNTIME_DECODER_HPP__
#define __RUNTIME_DECODER_HPP__

#include <opcode_table.hpp>

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace VMPilot {
namespace Runtime {
class Decoder {
   public:
    [[nodiscard]] static Decoder& GetInstance() noexcept;
    void Init(const std::string& key);

    [[nodiscard]] std::vector<uint8_t> Decode(const std::vector<uint8_t>& data);

   private:
    Decoder() = default;
    Decoder(const Decoder&) = delete;
    Decoder& operator=(const Decoder&) = delete;
    std::string key_;
    std::unique_ptr<Opcode_table> decode_table_;
};
};  // namespace Runtime
};  // namespace VMPilot

#endif