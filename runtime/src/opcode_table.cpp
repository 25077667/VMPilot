#include <instruction_t.hpp>
#include <opcode_table.hpp>

using Instruction_t = VMPilot::Runtime::Instruction_t;
using Opcode_t = decltype(Instruction_t::opcode);

Opcode_t VMPilot::Runtime::Opcode_table::find(Opcode_t opcode) const {
    const auto& opcode_entry = table.find(opcode);
    if (opcode_entry == table.end())
        throw std::runtime_error("Invalid instruction");
    return opcode_entry->second;
}