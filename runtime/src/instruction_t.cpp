#include <openssl/evp.h>
#include <cstring>
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
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    EVP_DecryptInit_ex(
        ctx, EVP_aes_256_cbc(), NULL,
        reinterpret_cast<const unsigned char*>(padded_key.data()), NULL);
    EVP_DecryptUpdate(ctx, reinterpret_cast<unsigned char*>(&inst), NULL,
                      reinterpret_cast<const unsigned char*>(data.data()),
                      sizeof(inst));
    EVP_DecryptFinal_ex(ctx, reinterpret_cast<unsigned char*>(&inst), NULL);
    EVP_CIPHER_CTX_free(ctx);
}

void VMPilot::Runtime::Instruction::update_checksum(
    Instruction_t& inst) noexcept {
    inst.checksum = detail::Hash(inst);
}

auto VMPilot::Runtime::Instruction::flatten(const Instruction_t& inst) noexcept
    -> std::array<uint8_t, sizeof(Instruction_t)> {
    using Instruction_t = VMPilot::Runtime::Instruction_t;

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
    EVP_MD_CTX* mdctx = EVP_MD_CTX_new();
    const EVP_MD* md = EVP_sha256();
    unsigned char md_value[EVP_MAX_MD_SIZE];
    unsigned int md_len;

    EVP_DigestInit_ex(mdctx, md, NULL);
    EVP_DigestUpdate(mdctx, data.c_str(), data.size());
    EVP_DigestUpdate(mdctx, salt.c_str(), salt.size());
    EVP_DigestFinal_ex(mdctx, md_value, &md_len);
    EVP_MD_CTX_free(mdctx);

    // Convert the hash to a number
    std::string hash_str;
    for (unsigned int i = 0; i < md_len; i++)
        hash_str += std::to_string(md_value[i]);

    return std::hash<std::string>{}(hash_str);
}
