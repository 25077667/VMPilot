#include <cstring>
#include <functional>

#include <crypto.hpp>
#include <instruction_t.hpp>

using Instruction_t = VMPilot::Common::Instruction_t;
using Hash_val_t = decltype(Instruction_t::checksum);

namespace detail {

/**
 * @brief Hash the instruction
 * 
 * We use SHA-256 from the OpenSSL library to hash the instruction.
 * Inputs: opcode, left_operand, right_operand
 * Salt: nounce field
 */
Hash_val_t Hash(const Instruction_t& inst) noexcept;
}  // namespace detail

bool VMPilot::Common::Instruction::check(const Instruction_t& inst) noexcept {
    return detail::Hash(inst) == inst.checksum;
}

void VMPilot::Common::Instruction::decrypt(Instruction_t& inst,
                                           const std::string& key) noexcept {
    // Decrypt the instruction using the key
    // We use AES-256-CBC from the OpenSSL library to decrypt the instruction.
    const auto data = flatten(inst);

    // If key is too short, we pad it with 0
    std::string padded_key = key;
    if (key.size() < 32)
        padded_key.resize(32, 0);
    else if (key.size() > 32)
        padded_key.resize(32);

    // Decrypt the instruction
    const auto decrypted_data =
        VMPilot::Common::Crypto::Decrypt_AES_256_CBC_PKCS7(
            std::vector<uint8_t>(data.begin(), data.end()), padded_key);
    ::memcpy(&inst, decrypted_data.data(), sizeof(Instruction_t));

    // Update the checksum
    update_checksum(inst);
}

void VMPilot::Common::Instruction::update_checksum(
    Instruction_t& inst) noexcept {
    inst.checksum = detail::Hash(inst);
}

auto VMPilot::Common::Instruction::flatten(const Instruction_t& inst) noexcept
    -> std::array<uint8_t, sizeof(Instruction_t)> {
    using Instruction_t = VMPilot::Common::Instruction_t;

    std::array<uint8_t, sizeof(Instruction_t)> result;
    ::memcpy(result.data(), &inst, sizeof(Instruction_t));
    return result;
}

Hash_val_t detail::Hash(const Instruction_t& inst) noexcept {
    // Hash the instruction
    std::string data;
    data += std::to_string(inst.opcode);
    data += std::to_string(inst.left_operand);
    data += std::to_string(inst.right_operand);
    // Salt of the hash
    std::string salt = std::to_string(inst.nounce);

    // Hash the data
    // VMPilot::Common::Crypto::SHA256();
    const auto& hash_str = VMPilot::Common::Crypto::SHA256(
        std::vector<uint8_t>(data.begin(), data.end()),
        std::vector<uint8_t>(salt.begin(), salt.end()));

    // hash that string
    return std::hash<std::string>{}(
        std::string(hash_str.begin(), hash_str.end()));
}
