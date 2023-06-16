#ifndef __RUNTIME_OPCODE_TABLE_HPP__
#define __RUNTIME_OPCODE_TABLE_HPP__

#include <instruction_t.hpp>

#include <unordered_map>

namespace VMPilot {
namespace Runtime {

class Opcode_table {
    using Opcode_t = decltype(Instruction_t::opcode);

   public:
    [[nodiscard]] Opcode_t find(Opcode_t opcode) const;

   private:
    std::unordered_map<Opcode_t, Opcode_t> table;
};

// Opcode_table_generator is a helper class to generate the decode table
class Opcode_table_generator {
   public:
    Opcode_table_generator(const std::string& key) noexcept;

    [[nodiscard]] std::unique_ptr<Opcode_table> Generate() noexcept;

   private:
    std::string key_;
};

};  // namespace Runtime
};  // namespace VMPilot

#endif