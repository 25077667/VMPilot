#include <openssl/sha.h>
#include <functional>
#include <instruction_t.hpp>

using Instruction_t = VMPilot::Runtime::Instruction_t;
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
};  // namespace detail

bool VMPilot::Runtime::Instruction::check(const Instruction_t& inst) noexcept {
    return detail::Hash(inst) == inst.checksum;
}

void VMPilot::Runtime::Instruction::decrypt(Instruction_t& inst,
                                            const std::string& key) noexcept {
    ;
}

void VMPilot::Runtime::Instruction::update_checksum(
    Instruction_t& inst) noexcept {
    inst.checksum = detail::Hash(inst);
}

auto VMPilot::Runtime::Instruction::flatten(const Instruction_t& inst) noexcept
    -> std::array<uint8_t, sizeof(Instruction_t)> {
    using Instruction_t = VMPilot::Runtime::Instruction_t;

    std::array<uint8_t, sizeof(Instruction_t)> result;
    std::memcpy(result.data(), &inst, sizeof(Instruction_t));
    return result;
}

Hash_val_t detail::Hash(const Instruction_t& inst) noexcept {
    // Using alias of the type of the instruction's members
    using Opcode_t = decltype(Instruction_t::opcode);
    using Left_operand_t = decltype(Instruction_t::left_operand);
    using Right_operand_t = decltype(Instruction_t::right_operand);

    // Hash the instruction
    std::string data;
    data += std::to_string(inst.opcode);
    data += std::to_string(inst.left_operand);
    data += std::to_string(inst.right_operand);
    // Salt of the hash
    std::string salt = std::to_string(inst.nonce);

    // Hash the data
    std::array<uint8_t, SHA256_DIGEST_LENGTH> hash_buffer;
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, data.c_str(), data.size());
    SHA256_Update(&sha256, salt.c_str(), salt.size());
    SHA256_Final(hash_buffer.data(), &sha256);

    // Convert the hash to a number
    std::string hash_str;
    for (const auto& byte : hash_buffer)
        hash_str += std::to_string(byte);

    return std::hash<std::string>{}(hash_str);
}