#include <openssl/evp.h>
#include <instruction_t.hpp>
#include <opcode_enum.hpp>
#include <opcode_table.hpp>

#include <map>
#include <utility>

using Instruction_t = VMPilot::Runtime::Instruction_t;
using Opcode_t = decltype(Instruction_t::opcode);
using OI = Opcode_t;
using OID = Opcode_t;

namespace detail {
// Two loop-up tables relates:
//      - RealOpcode to Opcode Index (OI)
//      - OID to OI
std::unordered_map<Opcode_t, OI> RealOp_to_OI;
std::unordered_map<OID, OI> OID_to_OI;

// SHA1
void three_way_table_init(const std::string& key);
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
    detail::three_way_table_init(key);
}

Opcode_t VMPilot::Runtime::Opcode_table_generator::GetOpcodeIndex(
    Opcode_t OID) const {
    const auto& opcode_entry = detail::OID_to_OI.find(OID);
    if (opcode_entry == detail::OID_to_OI.end())
        throw std::runtime_error("Invalid instruction");
    return opcode_entry->second;
}

std::unique_ptr<VMPilot::Runtime::Opcode_table>
VMPilot::Runtime::Opcode_table_generator::Generate() const noexcept {
    std::unordered_map<OI, Opcode_t> table;
    for (const auto& [opcode, OI] : detail::RealOp_to_OI)
        table[OI] = opcode;

    return std::make_unique<Opcode_table>(table);
}

void detail::three_way_table_init(const std::string& key) {
    // 1. For loop over the real opcodes from Opcode_enum.hpp
    // 2. Calculate the SHA1 hash of the opcode and the key(salt) inserting them into a list
    //    Now, we have a list of (SHA1, RealOpcode)
    // 3. Sort the list with the SHA1, array index is the OI
    // 4. For loop over the list and assign the index to the hash
    // 5. Assign for all OID = (start number) + array index
    //    The start number is the first element SHA1 last byte of the list
    // 6. Generate the OID_to_OI table
    const auto& salt = key;
    std::map<std::string, Opcode_t> list;

    // Step 1 to 3
    using namespace VMPilot::Runtime::Opcode::Enum;
    for (OpcodeBound i = OpcodeBound::__BEGIN; i != OpcodeBound::__END; ++i) {
        // It current opcode is over the last opcode, reset to the next starting opcode
        // Note: It is executed success as this sample link https://godbolt.org/z/PfWMfhETj
        if (i == DataMovement::__END)
            i = ArithmeticLogic::__BEGIN;
        else if (i == ArithmeticLogic::__END)
            i = ControlTransfer::__BEGIN;
        else if (i == ControlTransfer::__END)
            i = ThreadingAtomic::__BEGIN;
        else if (i == ThreadingAtomic::__END)
            break;

        auto& cur_op = i;
        auto& sha1 = get_Opcode_SHA1(cur_op, salt);
        list.insert(std::make_pair(std::move(sha1), cur_op));
    }

    // Step 4
    // Get the start number
    const auto& start_number = list.begin()->first.back();

    // Generate the RealOp_to_OI table and OID_to_OI table
    Opcode_t index = 0;
    for (const auto& [sha1, opcode] : list) {
        RealOp_to_OI.insert(std::make_pair(opcode, index));
        OID_to_OI.insert(std::make_pair(start_number + index, index));
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

    std::vector<uint8_t> result(EVP_MAX_MD_SIZE);
    unsigned int length = 0;
    EVP_DigestFinal_ex(context, result, &length);
    EVP_MD_CTX_free(context);

    return std::string(result.begin(), result.begin() + length);
}