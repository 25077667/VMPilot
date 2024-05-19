#ifndef __COMMON_INSTRUCTION_T_HPP__
#define __COMMON_INSTRUCTION_T_HPP__

#include <array>
#include <cstdint>
#include <string>

namespace VMPilot::Common {

struct Instruction_t {
    // Data layout of Instruction_t:
    //
    // (Opcode:         16 bits)        |
    // (Left Operand:   64 bits)        |
    // (Right Operand:  64 bits)        |
    // (Nonce:          32 bits)        |
    // (Checksum:       16 bits)        |
    //

    uint16_t opcode;
    uint64_t left_operand;
    uint64_t right_operand;
    uint32_t nounce;
    uint16_t checksum;
};

struct Instruction {

    /**
     * @brief Validates the instruction by checking its checksum.
     * 
     * The checksum is a hash of the instruction's opcode, operands, and nonce.
     * The nonce serves as the salt for the hash function.
     */
    [[nodiscard]] bool check(const Instruction_t& inst) noexcept;

    /**
     * @brief Decrypts the instruction using the provided key.
     * 
     * @param inst The instruction to decrypt.
     * @param key The decryption key.
     * 
     * Decrypts the instruction and updates its checksum.
     */
    void decrypt(Instruction_t& inst, const std::string& key) noexcept;

    /**
     * @brief Encrypts the instruction using the provided key.
     * 
     * @param inst The instruction to encrypt (the nonce field should NOT be set).
     * @param key The encryption key.
     * 
     * Encrypts the instruction, sets the nonce, and updates the checksum.
     */
    void encrypt(Instruction_t& inst, const std::string& key) noexcept;

    /**
     * @brief Encrypts the instruction using the provided key and nonce.
     * 
     * @param inst The instruction to encrypt (the nonce field should will be overwritten).
     * @param key The encryption key.
     * @param nonce The nonce to use for encryption.
     * 
     * Encrypts the instruction, uses the given nonce, and updates the checksum.
     */
    void encrypt(Instruction_t& inst, const std::string& key,
                 uint32_t nonce) noexcept;

    /**
     * @brief Updates the checksum of the instruction.
     * 
     * This function should only be used after decrypting the instruction.
     * ?Note: Consider if this function should be public.
     */
    void update_checksum(Instruction_t& inst) noexcept;

    /**
     * @brief Flattens the instruction into a byte array.
     * 
     * Converts the instruction into a contiguous byte array.
     */
    std::array<uint8_t, sizeof(Instruction_t)> flatten(
        const Instruction_t& inst) noexcept;
};  // struct Instruction helpers

}  // namespace VMPilot::Common

#endif
