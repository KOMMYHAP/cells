#pragma once

constexpr uint8_t ProcessorUnitSize = 1;

enum class ProcessorFlags : uint8_t {
    None = 1 << 0,
    InvalidCommand = 1 << 1,
    Equal = 1 << 2,
    Less = 1 << 3,
    Greater = 1 << 4
};

constexpr uint8_t ProcessRegistryCount = 3;

#pragma pack(push, 1)
struct ProcessorControlBlock {
    uint8_t flags = static_cast<uint8_t>(ProcessorFlags::None);
    uint8_t nextCommand { 0 };
    std::array<uint8_t, ProcessRegistryCount> registry {};
};
#pragma pack(pop)