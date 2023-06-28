#ifndef __COMMON_OPCODE_TABLE_HPP__
#define __COMMON_OPCODE_TABLE_HPP__

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

namespace VMPilot::Common {
using Opcode_t = decltype(Instruction_t::opcode);
using OI = Opcode_t;
using OID = Opcode_t;
using RealOpcode = Opcode_t;

using Runtime_OT = std::unordered_map<OI, RealOpcode>;
using Buildtime_OT = std::unordered_map<RealOpcode, OID>;
using OID_to_OI = std::unordered_map<OID, OI>;

class Opcode_table {
   public:
    Opcode_table() = default;
    Opcode_table(const std::unordered_map<OI, RealOpcode>& t_) : table(t_){};
    Opcode_table(std::unordered_map<OI, RealOpcode>&& t_)
        : table(std::move(t_)){};

    [[nodiscard]] RealOpcode find(OI oi) const;

   private:
    std::unordered_map<OI, RealOpcode> table;
};

// Opcode_table_generator is a helper class to generate the decode table
class Opcode_table_generator {
   public:
    Opcode_table_generator(const std::string& key) noexcept;

    [[nodiscard]] OI GetOpcodeIndex(Opcode_t OID) const;

    [[nodiscard]] Runtime_OT Generate() const noexcept;

    // A helper function that we will use it in the future (SDK part)
    [[nodiscard]] Buildtime_OT Get_RealOp_to_OID() const noexcept;

#ifdef DEBUG
    [[nodiscard]] OID_to_OI GetOID_to_OI() const noexcept;
#endif

   private:
    void three_way_table_init();

   private:
    std::string key_;
    Runtime_OT OI_to_RealOp_;
    Buildtime_OT RealOp_to_OID_;
    OID_to_OI OID_to_OI_;
};

}  // namespace VMPilot::Common

#endif
