#pragma once

namespace Details {

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
    ASSERT(HasBytes(offset), "Not enough memory");
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
    ASSERT(HasBytes<T>(), "Not enough memory");
    T value;
    memcpy(&value, _memory.data(), sizeof(T));
    return value;
}

template <class Unit>
MemoryBase<Unit>::MemoryBase(std::span<Unit> memory)
    : _memory(memory)
{
}

template <class Unit>
std::span<Unit> MemoryBase<Unit>::MakeSpan(uint8_t offset /*= 0*/) const
{
    return _memory.subspan(offset);
}

}

inline ProcessorMemory::ProcessorMemory(std::span<std::byte> memory)
    : MemoryBase(memory)
{
}

inline ProcessorConstMemory::ProcessorConstMemory(std::span<std::byte> memory)
    : MemoryBase(memory)
{
}

inline ProcessorConstMemory::ProcessorConstMemory(std::span<const std::byte> memory)
    : MemoryBase(memory)
{
}

template <MemoryType T>
T& ProcessorMemory::Access()
{
    ASSERT(HasBytes<T>(), "Not enough memory");
    T* value = reinterpret_cast<T*>(_memory.data());
    Move<T>();
    return *value;
}

template <MemoryType... Ts>
std::tuple<bool, Ts*...> ProcessorMemory::TryAccess()
{
    if (!HasBytes<Ts...>()) {
        return { false, static_cast<Ts*>(nullptr)... };
    }

    return { true, &Access<Ts>()... };
}

template <class... Ts>
    requires(MemoryType<std::decay_t<Ts>> && ...)
void ProcessorMemory::Write(Ts&&... ts)
{
    static_assert(sizeof...(ts) > 0, "Usage: memory.Write(ProcessorInstruction::Jump, std::byte{42});");
    (WriteOne(std::forward<Ts>(ts)), ...);
}

template <class... Ts>
    requires(MemoryType<std::decay_t<Ts>> && ...)
bool ProcessorMemory::TryWrite(Ts&&... ts)
{
    if (!HasBytes<std::decay_t<Ts>...>()) {
        return false;
    }
    Write(std::forward<Ts>(ts)...);
    return true;
}

template <class T>
    requires MemoryType<std::decay_t<T>>
void ProcessorMemory::WriteOne(T&& data)
{
    Access<std::decay_t<T>>() = std::forward<T>(data);
}