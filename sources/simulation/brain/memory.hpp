#pragma once

namespace Details {

template <class Unit>
bool MemoryBase<Unit>::HasBytes(uint8_t count) const
{
    return Size() >= count;
}
template <class Unit>
template <class T>
bool MemoryBase<Unit>::HasBytes() const
{
    return HasBytes(sizeof(T));
}

template <class Unit>
void MemoryBase<Unit>::Move(uint8_t offset)
{
    memory = memory.subspan(offset);
}

template <class Unit>
template <class T>
void MemoryBase<Unit>::Move()
{
    Move(sizeof(T));
}

template <class Unit>
template <class T>
const T& MemoryBase<Unit>::Get()
{
    assert(HasBytes<T>());
    const T& value = *reinterpret_cast<const T*>(memory.data());
    Move<T>();
    return value;
}

template <class Unit>
MemoryBase<Unit>::MemoryBase(std::span<Unit> memory)
    : memory(memory)
{
}

}

template <class T>
T& Memory::Pop()
{
    assert(HasBytes<T>());
    T& value = *reinterpret_cast<T*>(memory.data());
    Move<T>();
    return value;
}