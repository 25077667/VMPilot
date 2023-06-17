/**
 * Data Movement Instructions:
 *   MOV: Move data between registers, memory, and immediate values.
 *   PUSH: Push data onto the stack.
 *   POP: Pop data from the stack.
 *   LOAD: Load data from memory.
 *   STORE: Store data to memory.
 * 
 * Arithmetic and Logic Instructions:
 *   ADD, SUB, MUL, DIV: Basic arithmetic operations.
 *   AND, OR, XOR: Logical operations.
 *   CMP: Compare two values.
 * 
 * Control Transfer Instructions:
 *   JMP: Unconditional jump to a different location in the program.
 *   JZ, JNZ, JE, JNE: Conditional jumps based on the result of a comparison.
 *   CALL: Call a procedure or function.
 *   RET: Return from a procedure or function call.
 * 
 * Threading and Atomic Instructions:
 *   LOCK: Used as a prefix to certain instructions to make them atomic.
 *   XCHG: Atomic exchange operation.
 *   CMPXCHG: Compare and exchange atomically.
 *   LOCK_ADD, LOCK_SUB: Atomic addition and subtraction.
 *   FENCE: Memory barrier instructions for ordering memory operations.
 */

#ifndef __RUNTIME_OPCODE_ENUM_HPP__
#define __RUNTIME_OPCODE_ENUM_HPP__

#include <instruction_t.hpp>

#include <climits>
#include <cstddef>
#include <cstdint>
#include <type_traits>

namespace VMPilot::Runtime::Opcode::Enum {
using Opcode_t = decltype(Instruction_t::opcode);

namespace Internal {
template <typename Enum, typename T>
constexpr auto enumSize(Enum e_end) -> T {
    return static_cast<T>(e_end) - 1;
}

enum class DataMovement {
    __BEGIN = static_cast<Opcode_t>(1),
    MOV = __BEGIN,
    PUSH,
    POP,
    LOAD,
    STORE,
    __END,
};

enum class ArithmeticLogic {
    __BEGIN = static_cast<Opcode_t>(1) << (sizeof(Opcode_t) * CHAR_BIT / 4),
    ADD = __BEGIN,
    SUB,
    MUL,
    DIV,
    AND,
    OR,
    XOR,
    CMP,
    __END,
};

enum class ControlTransfer {
    __BEGIN = static_cast<Opcode_t>(1) << (sizeof(Opcode_t) * CHAR_BIT / 4 * 2),
    JMP = __BEGIN,
    JZ,
    JNZ,
    JE,
    JNE,
    CALL,
    RET,
    __END,
};

enum class ThreadingAtomic {
    __BEGIN = static_cast<Opcode_t>(1) << (sizeof(Opcode_t) * CHAR_BIT / 4 * 3),
    LOCK = __BEGIN,
    XCHG,
    CMPXCHG,
    LOCK_ADD,
    LOCK_SUB,
    FENCE,
    __END,
};

/**
 * @brief Opcode enum class
 * 
 * The bit level view of the opcode enum class (into sizeof(Opcode_t)) like:
 * low bit                                                       high bit
 * | DataMovement | ArithmeticLogic | ControlTransfer | ThreadingAtomic |
 * |--------------|-----------------|-----------------|-----------------|
 * | 0            | 0               | 0               | 0               |
 * 
 * Each enum class has a size of sizeof(Opcode_t) / 4.
 * 
 * Example: We would like a DataMovement::Move. And the opcode would be:
 * | DataMovement::Move | 0 | 0 | 0 |
 * 
 * Example: We would like a ArithmeticLogic::Add. And the opcode would be:
 * | 0 | ArithmeticLogic::Add | 0 | 0 |
 * 
 * Example: We would like a ControlTransfer::Jmp. And the opcode would be:
 * | 0 | 0 | ControlTransfer::Jmp | 0 |
 * 
 * Example: We would like a ThreadingAtomic::Lock. And the opcode would be:
 * | 0 | 0 | 0 | ThreadingAtomic::Lock |
 * 
 * CAUTION:
 * It is invalid, if it combine two or more enum classes.
 * 
 */

#define BOUND_GEN_HELPER(ec)                         \
    ec##_BEGIN = static_cast<Opcode_t>(ec::__BEGIN), \
    ec##_END = static_cast<Opcode_t>(ec::__END)

enum class OpcodeBound : Opcode_t {
    __BEGIN = 1,
    BOUND_GEN_HELPER(DataMovement),
    BOUND_GEN_HELPER(ArithmeticLogic),
    BOUND_GEN_HELPER(ControlTransfer),
    BOUND_GEN_HELPER(ThreadingAtomic),
    __END,
};

#undef BOUND_GEN_HELPER
};  // namespace Internal

using DataMovement = Internal::DataMovement;
using ArithmeticLogic = Internal::ArithmeticLogic;
using ControlTransfer = Internal::ControlTransfer;
using ThreadingAtomic = Internal::ThreadingAtomic;
using OpcodeBound = Internal::OpcodeBound;

template <typename Enum>
std::enable_if_t<std::is_same_v<Enum, DataMovement>,
                 OpcodeBound> constexpr enumToOpcodeBound(Enum e) {
    return static_cast<OpcodeBound>(e);
}

template <typename Enum>
std::enable_if_t<std::is_same_v<Enum, ArithmeticLogic>,
                 OpcodeBound> constexpr enumToOpcodeBound(Enum e) {
    return static_cast<OpcodeBound>(e);
}

template <typename Enum>
std::enable_if_t<std::is_same_v<Enum, ControlTransfer>,
                 OpcodeBound> constexpr enumToOpcodeBound(Enum e) {
    return static_cast<OpcodeBound>(e);
}

template <typename Enum>
std::enable_if_t<std::is_same_v<Enum, ThreadingAtomic>,
                 OpcodeBound> constexpr enumToOpcodeBound(Enum e) {
    return static_cast<OpcodeBound>(e);
}

inline OpcodeBound& operator++(OpcodeBound& opcode) {
    opcode = static_cast<OpcodeBound>(static_cast<Opcode_t>(opcode) + 1);
    return opcode;
}

template <typename Enum>
inline bool operator==(OpcodeBound opcode, Enum specific) {
    return (static_cast<Opcode_t>(opcode) == static_cast<Opcode_t>(specific));
}

template <typename Enum>
constexpr auto enumSize(Enum begin__ = Enum::__BEGIN, Enum end__ = Enum::__END)
    -> Opcode_t {
    return static_cast<Opcode_t>(end__) - static_cast<Opcode_t>(begin__) - 2;
}

};  // namespace VMPilot::Runtime::Opcode::Enum

#endif