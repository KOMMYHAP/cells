#pragma once

#include <cassert>

namespace Details {

template <class Unit>
class BrainDataBase {
public:
    BrainDataBase(std::span<Unit> memory);

    template <class T>
    const T& Get();

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

class BrainData : public Details::BrainDataBase<std::byte> {
public:
    BrainData(std::span<std::byte> memory);

    template <class T>
    T& Pop();
};

class ConstBrainData : public Details::BrainDataBase<const std::byte> {
public:
    ConstBrainData(std::span<const std::byte> memory);
};

#include "brain_data.hpp"