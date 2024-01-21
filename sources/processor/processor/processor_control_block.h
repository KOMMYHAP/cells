#pragma once


constexpr uint8_t ProcessorUnitSize = 1;

enum class ProcessorInstruction : uint8_t {
    Nope,
    Write,
    Compare,
    Decrement,
    Increment,
    Jump,
    JumpIfEqual,
    Call,
};

enum class ProcessorState : uint8_t {
    Good,
    InvalidCommand,
    MemoryCorrupted,
    UnknownProcedure,
};

enum class ProcessorFlags : uint8_t {
    None = 1 << 0,
    Equal = 1 << 1,
    Less = 1 << 2,
    Greater = 1 << 3
};

constexpr uint8_t ProcessRegistryCount = 3;

#pragma pack(push, 1)
struct ProcessorControlBlock {
    uint8_t state = static_cast<uint8_t>(ProcessorState::Good);
    uint8_t flags = static_cast<uint8_t>(ProcessorFlags::None);
    uint8_t nextCommand { 0 };
    std::array<std::byte, ProcessRegistryCount> registry {};
};
#pragma pack(pop)