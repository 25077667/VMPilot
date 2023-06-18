#include <decoder.hpp>
#include <nlohmann/json.hpp>
#include <opcode_table.hpp>

#include <iostream>
#include <string>

int main() {
    auto ot_gen = VMPilot::Common::Opcode_table_generator("test");
    auto runtime_table = ot_gen.Generate();
    auto buildtime_table = ot_gen.Get_RealOp_to_OID();
#ifdef DEBUG
    auto secret_conv = ot_gen.GetOID_to_OI();
#endif
    nlohmann::json j;
    j["runtime_table"] = nlohmann::json::array();
    j["buildtime_table"] = nlohmann::json::array();

    // Loop over the buildtime table
    for (const auto& [opcode, oid] : buildtime_table) {
        nlohmann::json entry = {
            {"real_opcode", opcode},
            {"OID", oid},
        };
        j["buildtime_table"].push_back(entry);
    }

    // Loop over the runtime table
    for (const auto& [oi, opcode] : runtime_table) {
        nlohmann::json entry = {
            {"OI", oi},
            {"real_opcode", opcode},
        };
        j["runtime_table"].push_back(entry);
    }
#ifdef DEBUG
    // Loop over the secret conversion table
    for (const auto& [oid, oi] : secret_conv) {
        nlohmann::json entry = {
            {"OID", oid},
            {"OI", oi},
        };
        j["secret_conversion_table"].push_back(entry);
    }
#endif
    std::cout << j.dump(4) << std::endl;

    return 0;
}