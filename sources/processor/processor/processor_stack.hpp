#pragma once

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
    ASSERT(CanPop<T>());
    _offset -= sizeof(T);
    T value;
    memcpy(&value, &_buffer[_offset], sizeof(T));
    return value;
}

template <class T>
    requires MemoryType<std::decay_t<T>>
void ProcessorStack::Push(T&& value)
{
    ASSERT(CanPush<std::decay_t<T>>());
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