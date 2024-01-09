#pragma once

#include <cassert>

class BrainData {
public:
    BrainData(std::span<std::byte> memory);

    template <class T>
    T& Pop();

    template <class T>
    void Move();

    void Move(uint8_t offset);

    uint8_t Size() const { return memory.size(); }

    template <class T>
    bool HasBytes() const;

    bool HasBytes(uint8_t count) const;

private:
    std::span<std::byte> memory;
};

template <class T>
T& BrainData::Pop()
{
    assert(HasBytes<T>());
    T& value = *reinterpret_cast<T*>(memory.data());
    Move<T>();
    return value;
}

template <class T>
void BrainData::Move()
{
    Move(sizeof(T));
}

template <class T>
bool BrainData::HasBytes() const
{
    return HasBytes(sizeof(T));
}