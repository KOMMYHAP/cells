#pragma once

namespace Details {

template <class Unit>
BrainBase<Unit>::BrainBase(Unit& cell)
    : _cell(cell)
{
}

template <class Unit>
ConstMemory BrainBase<Unit>::GetMemory() const
{
    const auto memory = std::span { std::as_const(_cell).brain, std::as_const(_cell).brain + Cell::brainSize };
    return ConstMemory { memory };
}

}