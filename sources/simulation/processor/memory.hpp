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
const T& MemoryBase<Unit>::Read()
{
    const T& value = Peek<T>();
    Move<T>();
    return value;
}

template <class Unit>
template <class T>
const T& MemoryBase<Unit>::Peek()
{
    assert(HasBytes<T>());
    return *reinterpret_cast<const T*>(memory.data());
}

template <class Unit>
MemoryBase<Unit>::MemoryBase(std::span<Unit> memory)
    : memory(memory)
{
}

template <class Unit>
std::span<Unit> MemoryBase<Unit>::MakeSubSpan(uint8_t bytesCount)
{
    return memory.subspan(0, bytesCount);
}

}

template <class T>
T& Memory::Get()
{
    assert(HasBytes<T>());
    T& value = *reinterpret_cast<T*>(memory.data());
    Move<T>();
    return value;
}

template <class... Args>
void Memory::Write(Args&&... args)
{
    if constexpr (sizeof...(args) > 0) {
        ((WriteOne(std::forward<Args>(args))), ...);
    }
}

template <class T>
void Memory::WriteOne(T&& data)
{
    static_assert(sizeof(T) == 1);
    Get<T>() = std::forward<T>(data);
}