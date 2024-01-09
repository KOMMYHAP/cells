#pragma once

namespace Details {

template <class Unit>
class MemoryBase {
public:
    MemoryBase(std::span<Unit> memory);

    template <class T>
    const T& Read();

    template <class T>
    const T& Peek();

    template <class T>
    void Move();

    void Move(uint8_t offset);

    uint8_t Size() const { return memory.size(); }

    template <class T>
    bool HasBytes() const;

    bool HasBytes(uint8_t count) const;

protected:
    std::span<Unit> memory;
};

}

class Memory : public Details::MemoryBase<std::byte> {
public:
    Memory(std::span<std::byte> memory);

    template <class T>
    T& Pop();
};

class ConstMemory : public Details::MemoryBase<const std::byte> {
public:
    ConstMemory(std::span<const std::byte> memory);
};

#include "memory.hpp"