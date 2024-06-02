#pragma once

#include "processor_memory_type.h"

namespace details {

template <class Unit>
class MemoryBase {
public:
    MemoryBase(std::span<Unit> memory);

    template <MemoryType T>
    T Read();

    template <MemoryType... Ts>
    std::tuple<bool, Ts...> TryRead();

    template <MemoryType T>
    T Peek();

    template <MemoryType T>
    void Move();

    void Move(uint8_t offset);

    uint8_t Size() const { return NarrowCast<uint8_t>(_memory.size()); }

    template <MemoryType... Ts>
    bool HasBytes() const;

    bool HasBytes(uint8_t count) const;

    std::span<Unit> MakeSpan(uint8_t offset = 0) const;

protected:
    std::span<Unit> _memory;
};

}

class ProcessorMemory : public details::MemoryBase<std::byte> {
public:
    ProcessorMemory(std::span<std::byte> memory);

    template <MemoryType T>
    T& Access();

    template <MemoryType... Ts>
    std::tuple<bool, Ts*...> TryAccess();

    template <class... Ts>
        requires(MemoryType<std::decay_t<Ts>> && ...)
    void Write(Ts&&... ts);

    template <class... Ts>
        requires(MemoryType<std::decay_t<Ts>> && ...)
    bool TryWrite(Ts&&... ts);

private:
    template <class Arg>
        requires MemoryType<std::decay_t<Arg>>
    void WriteOne(Arg&& data);
};

class ProcessorConstMemory : public details::MemoryBase<const std::byte> {
public:
    ProcessorConstMemory(std::span<std::byte> memory);
    ProcessorConstMemory(std::span<const std::byte> memory);
};

#include "processor_memory.hpp"