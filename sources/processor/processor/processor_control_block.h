#pragma once

#include "processor/instructions/instruction_list.h"
#include "processor_state.h"

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