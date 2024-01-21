#pragma once

namespace details {

template <class T>
concept MemoryType = !std::is_reference_v<T> && !std::is_pointer_v<T>;

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

    uint8_t Size() const { return _memory.size(); }

    template <MemoryType... Ts>
    bool HasBytes() const;

    bool HasBytes(uint8_t count) const;

    std::span<Unit> MakeSubSpan(uint8_t offset) const;

protected:
    std::span<Unit> _memory;
};

}

class Memory : public details::MemoryBase<std::byte> {
public:
    Memory(std::span<std::byte> memory);

    template <details::MemoryType T>
    T& Access();

    template <details::MemoryType... Ts>
    std::tuple<bool, Ts*...> TryAccess();

    template <class... Args>
    void Write(Args&&... args);

    template <class... Args>
    bool TryWrite(Args&&... args);

private:
    template <class Arg>
    void WriteOne(Arg&& data);
};

class ConstMemory : public details::MemoryBase<const std::byte> {
public:
    ConstMemory(std::span<std::byte> memory);
    ConstMemory(std::span<const std::byte> memory);
};

#include "memory.hpp"