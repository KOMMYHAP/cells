#pragma once

#include "brain/cell.h"

class Field;

class FieldSearchProxy {
public:
    FieldSearchProxy(Field& world, uint32_t width, uint32_t height, uint32_t bufferSize, uint32_t cellSize);
    FieldSearchProxy(const FieldSearchProxy&) = delete;
    FieldSearchProxy(FieldSearchProxy&&) = delete;
    ~FieldSearchProxy();

    std::span<const CellId> Find(const sf::Vector2u& position, uint32_t searchSizeLimit) const;

    void Add(CellId id);
    void Remove(CellId id);

private:
    template <class Impl>
    Impl& As()
    {
        return *reinterpret_cast<Impl*>(_quadtreeMemory);
    }
    template <class Impl>
    const Impl& As() const
    {
        return *reinterpret_cast<const Impl*>(_quadtreeMemory);
    }

private:
    Field& _world;
    mutable std::vector<CellId> _searchBuffer;
    uint32_t _cellSize;

    constexpr static uint32_t _quadTreeMemorySize = 40;
    constexpr static uint32_t _quadTreeAlignment = 8;
    alignas(_quadTreeAlignment) std::byte _quadtreeMemory[_quadTreeMemorySize];
};
