#pragma once

#include "processor_memory_type.h"

class ProcessorStack {
public:
    ProcessorStack(const std::span<std::byte>& buffer, uint8_t & offset);

    template <class T>
        requires MemoryType<std::decay_t<T>>
    void Push(T&& value);

    template <MemoryType T>
    T Pop();

    template <class... Ts>
        requires(MemoryType<std::decay_t<Ts>> && ...)
    bool TryPush(Ts&&... ts);

    template <MemoryType... Ts>
    std::tuple<bool, Ts...> TryPop();

    template <MemoryType... Ts>
    bool CanPush() const;

    bool CanPush(uint8_t bytes) const;

    template <MemoryType... Ts>
    bool CanPop() const;

    bool CanPop(uint8_t count) const;

    uint8_t GetPushedBytesCount() const;
    uint8_t GetBytesCapacity() const;

private:
    std::span<std::byte> _buffer;
    uint8_t& _offset;
};

#include "processor_stack.hpp"