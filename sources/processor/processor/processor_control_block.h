#pragma once

enum class ProcessorFlags : uint8_t {
    Zero = 1 << 0,
    Sign = 1 << 1,
    Overflow = 1 << 2,
    Carry = 1 << 3,
    Parity = 1 << 4
};

constexpr uint8_t ProcessorRegistryCount = 4;
constexpr uint8_t ProcessorStackSize = 5;

struct ProcessorControlBlock {
    uint8_t state;
    uint8_t pendingProcedureId;
    uint8_t flags;
    uint8_t nextCommand;
    std::array<std::byte, ProcessorRegistryCount> registry;
    uint8_t stackOffset;
    std::array<std::byte, ProcessorStackSize> stack;
};
