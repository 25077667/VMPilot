#include <blake3.h>
#include <instruction_t.hpp>
#include <opcode_enum.hpp>
#include <opcode_table.hpp>

#include <map>
#include <stdexcept>
#include <utility>

using Instruction_t = VMPilot::Common::Instruction_t;
using Opcode_t = decltype(Instruction_t::opcode);
using OI = Opcode_t;
using OID = Opcode_t;
using RealOpcode = Opcode_t;

using Runtime_OT = std::unordered_map<OI, RealOpcode>;
using Buildtime_OT = std::unordered_map<RealOpcode, OID>;
using OID_to_OI = std::unordered_map<OID, OI>;

namespace detail {
std::string get_Opcode_BLAKE3(Opcode_t opcode,
                              const std::string& salt) noexcept;
};  // namespace detail

RealOpcode VMPilot::Common::Opcode_table::find(OI oi) const {
    const auto& opcode_entry = table.find(oi);
    if (opcode_entry == table.end())
        throw std::runtime_error("Invalid instruction");
    return opcode_entry->second;
}

VMPilot::Common::Opcode_table_generator::Opcode_table_generator(
    const std::string& key) noexcept
    : key_(key) {
    this->three_way_table_init();
}

Opcode_t VMPilot::Common::Opcode_table_generator::GetOpcodeIndex(
    Opcode_t OID) const {
    OID_to_OI::const_iterator it = OID_to_OI_.find(OID);
    if (it == OID_to_OI_.end())
        throw std::runtime_error("Invalid instruction");
    return it->second;
}

Runtime_OT VMPilot::Common::Opcode_table_generator::Generate() const noexcept {
    return OI_to_RealOp_;
}

Buildtime_OT VMPilot::Common::Opcode_table_generator::Get_RealOp_to_OID()
    const noexcept {
    return RealOp_to_OID_;
}

#ifdef DEBUG
OID_to_OI VMPilot::Common::Opcode_table_generator::GetOID_to_OI()
    const noexcept {
    return OID_to_OI_;
}
#endif

void VMPilot::Common::Opcode_table_generator::three_way_table_init() {
    // 1. For loop over the real opcodes from Opcode_enum.hpp
    // 2. Calculate the BLAKE3 hash of the opcode and the key(salt) inserting them into a list
    //    Now, we have a list of (BLAKE3, RealOpcode)
    // 3. Sort the list with the BLAKE3, array index is the OI
    // 4. For loop over the list and assign the index to the hash
    // 5. Assign for all OID = (start number) + array index
    //    The start number is the first element BLAKE3 last byte of the list
    // 6. Generate the OID_to_OI table
    std::map<std::string, Opcode_t> list;

    // Step 1 to 3
    using namespace VMPilot::Common::Opcode::Enum;
    for (auto i = OpcodeBound::__BEGIN; i != OpcodeBound::__END; ++i) {
        // It current opcode is over the last opcode, reset to the next starting opcode
        // Note: It is executed success as this sample link https://godbolt.org/z/PfWMfhETj
        if (i == DataMovement::__END)
            i = enumToOpcodeBound(ArithmeticLogic::__BEGIN);
        else if (i == ArithmeticLogic::__END)
            i = enumToOpcodeBound(ControlTransfer::__BEGIN);
        else if (i == ControlTransfer::__END)
            i = enumToOpcodeBound(ThreadingAtomic::__BEGIN);
        else if (i == ThreadingAtomic::__END)
            break;

        const auto& cur_op = static_cast<Opcode_t>(i);
        const auto& blake3 = detail::get_Opcode_BLAKE3(cur_op, key_);
        list.insert(std::make_pair(std::move(blake3), cur_op));
    }

    // Step 4
    // Get the start number
    const auto& start_number = list.begin()->first.back();

    // Generate the RealOp_to_OI table and OID_to_OI table
    Opcode_t index = 0;
    for (const auto& [_, opcode] : list) {
        const auto OID = start_number + index;
        OI_to_RealOp_.insert(std::make_pair(index, opcode));
        RealOp_to_OID_.insert(std::make_pair(opcode, OID));
        OID_to_OI_.insert(std::make_pair(OID, index));

        ++index;
    }
}

std::string detail::get_Opcode_BLAKE3(Opcode_t opcode,
                                      const std::string& salt) noexcept {
    // To string
    const auto& opcode_str = std::to_string(opcode) + salt;

    blake3_hasher hasher;
    blake3_hasher_init(&hasher);

    // Hash the opcode
    blake3_hasher_update(&hasher, opcode_str.c_str(), opcode_str.size());
    // Finalize the hash. BLAKE3_OUT_LEN is the default output length, 32 bytes.
    uint8_t result[BLAKE3_OUT_LEN];
    blake3_hasher_finalize(&hasher, result, BLAKE3_OUT_LEN);

    return std::string(result, result + BLAKE3_OUT_LEN);
}
