#pragma once

inline bool ProcessorStack::CanPush(uint8_t bytes) const
{
    return GetPushedBytesCount() + bytes <= GetBytesCapacity();
}

inline bool ProcessorStack::CanPop(uint8_t count) const
{
    return count <= GetPushedBytesCount();
}

template <MemoryType... Ts>
std::tuple<bool, Ts...> ProcessorStack::TryPop()
{
    if (!CanPop<Ts...>()) {
        return { false, Ts {}... };
    }
    return { true, Pop<Ts>()... };
}

template <MemoryType T>
T ProcessorStack::Pop()
{
    ASSUME(CanPop<T>());
    _offset -= sizeof(T);
    T value;
    memcpy(&value, &_buffer[_offset], sizeof(T));
    return value;
}

template <class T>
    requires MemoryType<std::decay_t<T>>
void ProcessorStack::Push(T&& value)
{
    ASSUME(CanPush<std::decay_t<T>>());
    new (&_buffer[_offset]) std::decay_t<T>(std::forward<T>(value));
    _offset += sizeof(T);
}

template <class... Ts>
    requires(MemoryType<std::decay_t<Ts>> && ...)
bool ProcessorStack::TryPush(Ts&&... ts)
{
    if (!CanPush<std::decay_t<Ts>...>()) {
        return false;
    }
    (Push(std::forward<Ts>(ts)), ...);
    return true;
}

inline ProcessorStack::ProcessorStack(const std::span<std::byte>& buffer, uint8_t& offset)
    : _buffer(buffer)
    , _offset(offset)
{
}

inline uint8_t ProcessorStack::GetPushedBytesCount() const
{
    return _offset;
}

inline uint8_t ProcessorStack::GetBytesCapacity() const
{
    return _buffer.size();
}

template <MemoryType... Ts>
bool ProcessorStack::CanPop() const
{
    return CanPop((sizeof(Ts) + ...));
}

template <MemoryType... Ts>
bool ProcessorStack::CanPush() const
{
    return CanPush((sizeof(Ts) + ...));
}