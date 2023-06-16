#ifndef __RUNTIME_INSTRUCTION_T_HPP__
#define __RUNTIME_INSTRUCTION_T_HPP__

#include <array>
#include <cstdint>
#include <string>

namespace VMPilot {
namespace Runtime {

struct Instruction_t {
    // Instruction_t Data layout:
    //
    // (Opcode:         16 bits)        |
    // (left operand:   64 bits)        |
    // (right operand:  64 bits)        |
    // (Nounce:         32 bits)        |
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
     * @brief Check if the instruction is valid.
     * 
     * Checking the checksum is correct.
     * Checksum is the hash of the instruction's opcode, operands and nounce.
     * The nounce is the salt of the hash function here.
     */
    [[nodiscard]] bool check(const Instruction_t& inst) noexcept;

    /**
     * @brief Decrypt the instruction.
     * 
     * Note: Also need to update the checksum.
     */
    void decrypt(Instruction_t& inst, const std::string& key) noexcept;

    /**
     * @brief Update the checksum of the instruction.
     * 
     * This function only should be used after decrypting the instruction.
     * ? Should this function be public?
     */
    void update_checksum(Instruction_t& inst) noexcept;

    /**
     * @brief Flatten the instruction into a byte array.
     */
    std::array<uint8_t, sizeof(Instruction_t)> flatten(
        const Instruction_t& inst) noexcept;
};  // struct Instruction helpers

};  // namespace Runtime
};  // namespace VMPilot
#endif