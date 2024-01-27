#pragma once

template <class Func>
void PositionSystem::Iterate(Func&& func)
{
    _grid.Iterate(std::forward<Func>(func));
}