#pragma once

namespace details {

template <class Unit>
bool MemoryBase<Unit>::HasBytes(uint8_t count) const
{
    return Size() >= count;
}

template <class Unit>
template <MemoryType... Ts>
bool MemoryBase<Unit>::HasBytes() const
{
    constexpr uint8_t bytes = (sizeof(Ts) + ...);
    return HasBytes(bytes);
}

template <class Unit>
void MemoryBase<Unit>::Move(uint8_t offset)
{
    _memory = _memory.subspan(offset);
}

template <class Unit>
template <MemoryType T>
void MemoryBase<Unit>::Move()
{
    Move(sizeof(T));
}

template <class Unit>
template <MemoryType T>
T MemoryBase<Unit>::Read()
{
    const T value = Peek<T>();
    Move<T>();
    return value;
}

template <class Unit>
template <MemoryType... Ts>
std::tuple<bool, Ts...> MemoryBase<Unit>::TryRead()
{
    if (!HasBytes<Ts...>()) {
        return { false, Ts {}... };
    }
    return { true, Read<Ts>()... };
}

template <class Unit>
template <MemoryType T>
T MemoryBase<Unit>::Peek()
{
    assert(HasBytes<T>());
    return *reinterpret_cast<const T*>(_memory.data());
}

template <class Unit>
MemoryBase<Unit>::MemoryBase(std::span<Unit> memory)
    : _memory(memory)
{
}

template <class Unit>
std::span<Unit> MemoryBase<Unit>::MakeSubSpan(uint8_t offset) const
{
    return _memory.subspan(offset);
}

}

template <details::MemoryType T>
T& Memory::Access()
{
    assert(HasBytes<T>());
    T& value = *reinterpret_cast<std::decay_t<T>*>(_memory.data());
    Move<T>();
    return value;
}

template <details::MemoryType... Ts>
std::tuple<bool, Ts*...> Memory::TryAccess()
{
    if (!HasBytes<Ts...>()) {
        return { false, static_cast<Ts*>(nullptr)... };
    }

    return { true, &Access<Ts>()... };
}

template <class... Args>
void Memory::Write(Args&&... args)
{
    static_assert(sizeof...(args) > 0, "Usage: memory.Write(ProcessorInstruction::Jump, std::byte{42});");
    (WriteOne(std::forward<Args>(args)), ...);
}

template <class... Args>
bool Memory::TryWrite(Args&&... args)
{
    if (!HasBytes<std::decay_t<Args>...>()) {
        return false;
    }
    Write(std::forward<Args>(args)...);
    return true;
}

template <class Arg>
void Memory::WriteOne(Arg&& data)
{
    Access<std::decay_t<Arg>>() = std::forward<Arg>(data);
}