#pragma once

namespace Details {

template <class Unit>
BrainBase<Unit>::BrainBase(Unit& cell)
    : _cell(cell)
{
    assert(Cell::brainSize - sizeof(CellInfo) == GetMemory().Size());
}

template <class Unit>
const CellInfo& BrainBase<Unit>::GetInfo() const
{
    return *reinterpret_cast<const CellInfo*>(_cell.brain);
}

template <class Unit>
ConstMemory BrainBase<Unit>::GetMemory() const
{
    const auto memory = std::span { std::as_const(_cell).brain + sizeof(CellInfo), std::as_const(_cell).brain + Cell::brainSize };
    return ConstMemory { memory };
}

}