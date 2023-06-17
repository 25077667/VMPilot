#include <decoder.hpp>
#include <instruction_t.hpp>
#include <opcode_table.hpp>

#include <exception>
#include <stdexcept>
#include <unordered_map>

using Instruction_t = VMPilot::Runtime::Instruction_t;
using Opcode_table_t = VMPilot::Runtime::Opcode_table;

namespace detail {
Instruction_t Fetch(const std::vector<uint8_t>& data, size_t offset) noexcept;

std::unique_ptr<VMPilot::Runtime::Opcode_table_generator> OTgen_(nullptr);
}  // namespace detail

VMPilot::Runtime::Decoder& VMPilot::Runtime::Decoder::GetInstance() noexcept {
    static Decoder instance;
    return instance;
}

void VMPilot::Runtime::Decoder::Init(const std::string& key) {
    key_ = key;

    // Initialize the opcode table generator
    detail::OTgen_ = std::make_unique<Opcode_table_generator>(key_);
    decode_table_ = std::make_unique<Opcode_table>(detail::OTgen_->Generate());
}

std::vector<uint8_t> VMPilot::Runtime::Decoder::Decode(
    const std::vector<uint8_t>& data) {
    if (data.size() % sizeof(Instruction_t) != 0)
        throw std::runtime_error("Invalid data size");

    std::vector<uint8_t> result;

    // Loop over the data, each iteration decode one instruction
    for (size_t i = 0; i < data.size(); i += sizeof(Instruction_t)) {
        Instruction_t inst = detail::Fetch(data, i);
        Instruction inst_helper;

        // Check if the instruction is valid
        if (!inst_helper.check(inst))
            throw std::runtime_error("Invalid instruction");

        // Decrypt the instruction
        inst_helper.decrypt(inst, key_);

        // Find the real opcode
        const auto& OID = inst.opcode;
        const auto& OI = detail::OTgen_->GetOpcodeIndex(OID);
        inst.opcode = decode_table_->find(OI);

        inst_helper.update_checksum(inst);

        // Append the decrypted instruction to the result
        const auto flattened_inst = inst_helper.flatten(inst);
        result.insert(result.end(), flattened_inst.begin(),
                      flattened_inst.end());
    }

    return result;
}

Instruction_t detail::Fetch(const std::vector<uint8_t>& data,
                            size_t offset) noexcept {

    // Using alias of the type of the instruction's members
    using Opcode_t = decltype(Instruction_t::opcode);
    using Left_operand_t = decltype(Instruction_t::left_operand);
    using Right_operand_t = decltype(Instruction_t::right_operand);
    using Nounce_t = decltype(Instruction_t::nounce);
    using Checksum_t = decltype(Instruction_t::checksum);

    Instruction_t inst;
    inst.opcode =
        static_cast<Opcode_t>(data[offset + 0] << 8 | data[offset + 1]);
    inst.left_operand = static_cast<Left_operand_t>(data[offset + 2]) << 56 |
                        static_cast<Left_operand_t>(data[offset + 3]) << 48 |
                        static_cast<Left_operand_t>(data[offset + 4]) << 40 |
                        static_cast<Left_operand_t>(data[offset + 5]) << 32 |
                        static_cast<Left_operand_t>(data[offset + 6]) << 24 |
                        static_cast<Left_operand_t>(data[offset + 7]) << 16 |
                        static_cast<Left_operand_t>(data[offset + 8]) << 8 |
                        static_cast<Left_operand_t>(data[offset + 9]);
    inst.right_operand = static_cast<Right_operand_t>(data[offset + 10]) << 56 |
                         static_cast<Right_operand_t>(data[offset + 11]) << 48 |
                         static_cast<Right_operand_t>(data[offset + 12]) << 40 |
                         static_cast<Right_operand_t>(data[offset + 13]) << 32 |
                         static_cast<Right_operand_t>(data[offset + 14]) << 24 |
                         static_cast<Right_operand_t>(data[offset + 15]) << 16 |
                         static_cast<Right_operand_t>(data[offset + 16]) << 8 |
                         static_cast<Right_operand_t>(data[offset + 17]);
    inst.nounce = static_cast<Nounce_t>(data[offset + 18]) << 24 |
                  static_cast<Nounce_t>(data[offset + 19]) << 16 |
                  static_cast<Nounce_t>(data[offset + 20]) << 8 |
                  static_cast<Nounce_t>(data[offset + 21]);
    inst.checksum = static_cast<Checksum_t>(data[offset + 22]) << 8 |
                    static_cast<Checksum_t>(data[offset + 23]);
    return inst;
}