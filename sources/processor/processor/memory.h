#pragma once

#include <span>

namespace Details {

template <class Unit>
class MemoryBase {
public:
    MemoryBase(std::span<Unit> memory);

    template <class T>
    T Read();

    template <class... Ts>
    std::tuple<bool, Ts...> TryRead();

    template <class T>
    T Peek();

    template <class T>
    void Move();

    void Move(uint8_t offset);

    uint8_t Size() const { return memory.size(); }

    template <class... Ts>
    bool HasBytes() const;

    bool HasBytes(uint8_t count) const;

    std::span<Unit> MakeSubSpan(uint8_t bytesCount) const;

protected:
    std::span<Unit> memory;
};

}

class Memory : public Details::MemoryBase<std::byte> {
public:
    Memory(std::span<std::byte> memory);

    template <class T>
    T& Get();

    template <class... Args>
    void Write(Args&&... args);

    template <class... Args>
    bool TryWrite(Args&&... args);

private:
    template <class T>
    void WriteOne(T&& data);
};

class ConstMemory : public Details::MemoryBase<const std::byte> {
public:
    ConstMemory(std::span<std::byte> memory);
    ConstMemory(std::span<const std::byte> memory);
};

#include "memory.hpp"