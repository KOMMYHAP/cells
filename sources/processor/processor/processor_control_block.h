#pragma once

#include "processor_state.h"

enum class ProcessorInstruction : uint8_t {
    Nope,
    Write,
    Compare,
    Add,
    Jump,
    JumpIfEqual,
    Call, ///< Procedure arguments are passed on the stack
    PushStack,
    PopStack,
    LastProcessorInstruction
};
constexpr uint8_t ProcessorInstructionCount = static_cast<uint8_t>(ProcessorInstruction::LastProcessorInstruction);

enum class ProcessorFlags : uint8_t {
    None = 1 << 0,
    Equal = 1 << 1,
    Less = 1 << 2,
    Greater = 1 << 3
};

constexpr uint8_t ProcessorRegistryCount = 4;
constexpr uint8_t ProcessorStackSize = 5;

#pragma pack(push, 1)
struct ProcessorControlBlock {
    uint8_t state;
    uint8_t flags;
    uint8_t nextCommand;
    std::array<std::byte, ProcessorRegistryCount> registry;
    uint8_t stackOffset;
    std::array<std::byte, ProcessorStackSize> stack;
};
#pragma pack(pop)