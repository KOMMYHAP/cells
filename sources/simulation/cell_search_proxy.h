#pragma once

#include "cell.h"

class Field;

class CellSearchProxy {
public:
    CellSearchProxy(Field& world, uint32_t width, uint32_t height);
    CellSearchProxy(const CellSearchProxy&) = delete;
    CellSearchProxy(CellSearchProxy&&) = delete;
    ~CellSearchProxy();

    std::vector<CellId> Find(const sf::Vector2u& position) const;

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

    constexpr static uint32_t _quadTreeMemorySize = 32;
    constexpr static uint32_t _quadTreeAlignment = 8;
    alignas(_quadTreeAlignment) std::byte _quadtreeMemory[_quadTreeMemorySize];
};
