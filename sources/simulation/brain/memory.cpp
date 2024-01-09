#include "memory.h"
#include "cell.h"

Memory::Memory(std::span<std::byte> memory)
    : MemoryBase(memory)
{
}
Memory::Memory(Cell& cell)
    : Memory(cell.brain)
{
}

ConstMemory::ConstMemory(std::span<const std::byte> memory)
    : MemoryBase(memory)
{
}

ConstMemory::ConstMemory(const Cell& cell)
    : ConstMemory(cell.brain)
{
}
