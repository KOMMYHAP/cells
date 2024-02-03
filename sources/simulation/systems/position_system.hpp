#pragma once

template <class Func>
    requires std::invocable<Func, CellPosition>
void PositionSystem::Iterate(Func&& func) const
{
    for (int16_t y { 0 }; y < _height; ++y) {
        for (int16_t x { 0 }; x < _width; ++x) {
            func(CellPosition { x, y });
        }
    }
}
