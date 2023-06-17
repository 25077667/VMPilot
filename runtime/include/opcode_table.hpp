#ifndef __RUNTIME_OPCODE_TABLE_HPP__
#define __RUNTIME_OPCODE_TABLE_HPP__

#include <instruction_t.hpp>

#include <memory>
#include <string>
#include <unordered_map>

/**
 * @brief The opcode table mechanism
 * 
 * TL;DR
 * SHA1(RealOpcode, salt) -> OID -> OI -> RealOpcode
 * Opcode_table is using (OI, RealOpcode) pairs to retrieve the real opcode.
 * 
 * The Real opcode (RealOpcode) refers to the opcode_enum.hpp file.
 * ================================================================================
 * 
 * The opcode table mechanism retrieves the real opcode by following these steps:
 * 1. Obtain the Opcode ID (OID) from binary source data
 *    using the decoder mechanism. (see decoder.hpp)
 * 2. We use SHA1, the salt is the key string from the decoder,
 *    to the OID to calculate the corresponding Opcode Index (OI).
 * 3. Use the Opcode Index (OI) to locate the real opcode in the opcode table.
 * 
 * 
 */

namespace VMPilot::Runtime {
using Opcode_t = decltype(Instruction_t::opcode);
using OI = Opcode_t;
using OID = Opcode_t;

class Opcode_table {
   public:
    Opcode_table() = default;
    Opcode_table(const std::unordered_map<OI, Opcode_t>& t_) : table(t_){};
    Opcode_table(std::unordered_map<OI, Opcode_t>&& t_)
        : table(std::move(t_)){};

    [[nodiscard]] Opcode_t find(Opcode_t opcode) const;

   private:
    std::unordered_map<OI, Opcode_t> table;
};

// Opcode_table_generator is a helper class to generate the decode table
class Opcode_table_generator {
   public:
    Opcode_table_generator(const std::string& key) noexcept;

    [[nodiscard]] OI GetOpcodeIndex(Opcode_t OID) const;

    [[nodiscard]] std::unique_ptr<Opcode_table> Generate() const noexcept;

    // A helper function that we will use it in the future (SDK part)
    [[nodiscard]] std::unique_ptr<std::unordered_map<Opcode_t, Opcode_t>>
    Generate_RealOp_to_OID() const noexcept;

   private:
    std::string key_;
};

};  // namespace VMPilot::Runtime

#endif