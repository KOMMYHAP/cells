#include "memory.h"

Memory::Memory(std::span<std::byte> memory)
    : MemoryBase(memory)
{
}

ConstMemory::ConstMemory(std::span<std::byte> memory)
    : MemoryBase(memory)
{
}

ConstMemory::ConstMemory(std::span<const std::byte> memory)
    : MemoryBase(memory)
{
}
