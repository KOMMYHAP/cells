#pragma once

#include "processor_state.h"

enum class ProcessorInstruction : uint8_t {
    Nope,
    Write,
    Compare,
    Add,
    Jump,
    JumpIfEqual,
    Call,
};

enum class ProcessorFlags : uint8_t {
    None = 1 << 0,
    Equal = 1 << 1,
    Less = 1 << 2,
    Greater = 1 << 3
};

constexpr uint8_t ProcessRegistryCount = 6;

#pragma pack(push, 1)
struct ProcessorControlBlock {
    uint8_t state;
    uint8_t flags;
    uint8_t nextCommand;
    std::array<std::byte, ProcessRegistryCount> registry;
};
#pragma pack(pop)