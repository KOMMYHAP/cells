#pragma once

#include "cell.h"

class World;

class CellSearchProxy {
public:
    CellSearchProxy(World& world, uint32_t width, uint32_t height);
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
    World& _world;

    constexpr static uint32_t _quadTreeMemorySize = 40;
    constexpr static uint32_t _quadTreeAlignment = 8;
    alignas(_quadTreeAlignment) std::byte _quadtreeMemory[_quadTreeMemorySize];
};
