#include <openssl/evp.h>
#include <instruction_t.hpp>
#include <opcode_enum.hpp>
#include <opcode_table.hpp>

#include <map>
#include <stdexcept>
#include <utility>

using Instruction_t = VMPilot::Runtime::Instruction_t;
using Opcode_t = decltype(Instruction_t::opcode);
using OI = Opcode_t;
using OID = Opcode_t;
using RealOpcode = Opcode_t;

using Runtime_OT = std::unordered_map<OI, RealOpcode>;
using Buildtime_OT = std::unordered_map<RealOpcode, OID>;
using OID_to_OI = std::unordered_map<OID, OI>;

namespace detail {
// SHA1
std::string get_Opcode_SHA1(Opcode_t opcode, const std::string& salt) noexcept;
};  // namespace detail

Opcode_t VMPilot::Runtime::Opcode_table::find(Opcode_t opcode) const {
    const auto& opcode_entry = table.find(opcode);
    if (opcode_entry == table.end())
        throw std::runtime_error("Invalid instruction");
    return opcode_entry->second;
}

VMPilot::Runtime::Opcode_table_generator::Opcode_table_generator(
    const std::string& key) noexcept
    : key_(key) {
    this->three_way_table_init();
}

Opcode_t VMPilot::Runtime::Opcode_table_generator::GetOpcodeIndex(
    Opcode_t OID) const {
    OID_to_OI::const_iterator it = OID_to_OI_.find(OID);
    if (it == OID_to_OI_.end())
        throw std::runtime_error("Invalid instruction");
    return it->second;
}

Runtime_OT VMPilot::Runtime::Opcode_table_generator::Generate() const noexcept {
    return OI_to_RealOp_;
}

Buildtime_OT VMPilot::Runtime::Opcode_table_generator::Get_RealOp_to_OID()
    const noexcept {
    return RealOp_to_OID_;
}

#ifdef DEBUG
OID_to_OI VMPilot::Runtime::Opcode_table_generator::GetOID_to_OI()
    const noexcept {
    return OID_to_OI_;
}
#endif

void VMPilot::Runtime::Opcode_table_generator::three_way_table_init() {
    // 1. For loop over the real opcodes from Opcode_enum.hpp
    // 2. Calculate the SHA1 hash of the opcode and the key(salt) inserting them into a list
    //    Now, we have a list of (SHA1, RealOpcode)
    // 3. Sort the list with the SHA1, array index is the OI
    // 4. For loop over the list and assign the index to the hash
    // 5. Assign for all OID = (start number) + array index
    //    The start number is the first element SHA1 last byte of the list
    // 6. Generate the OID_to_OI table
    std::map<std::string, Opcode_t> list;

    // Step 1 to 3
    using namespace VMPilot::Runtime::Opcode::Enum;
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
        const auto& sha1 = detail::get_Opcode_SHA1(cur_op, key_);
        list.insert(std::make_pair(std::move(sha1), cur_op));
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

std::string detail::get_Opcode_SHA1(Opcode_t opcode,
                                    const std::string& salt) noexcept {
    // To string
    const auto& opcode_str = std::to_string(opcode);
    EVP_MD_CTX* context = EVP_MD_CTX_new();
    EVP_DigestInit_ex(context, EVP_sha1(), nullptr);
    EVP_DigestUpdate(context, opcode_str.c_str(), opcode_str.size());
    EVP_DigestUpdate(context, salt.c_str(), salt.size());

    unsigned char result[EVP_MAX_MD_SIZE];
    unsigned int length = 0;
    EVP_DigestFinal_ex(context, result, &length);
    EVP_MD_CTX_free(context);

    return std::string(result, result + length);
}