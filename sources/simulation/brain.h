#pragma once

#include <utility>

#include "brain_data.h"
#include "brain_packed_data.h"
#include "cell.h"

class Field;

template <class Unit>
class BrainViewBase {
public:
    BrainViewBase(Unit& cell)
        : _cell(cell)
    {
        assert(Cell::brainSize - sizeof(BrainInfo) == GetData().Size());
    }

    const BrainInfo& GetInfo() const
    {
        return *reinterpret_cast<const BrainInfo*>(_cell.brain);
    }

    ConstBrainData GetData() const
    {
        const auto memory = std::span { std::as_const(_cell).brain + sizeof(BrainInfo), std::as_const(_cell).brain + Cell::brainSize };
        return ConstBrainData { memory };
    }

protected:
    Unit& _cell;
};

class Brain : public BrainViewBase<Cell> {
public:
    Brain(Cell& cell)
        : BrainViewBase(cell)
    {
    }

    BrainInfo& AccessInfo() { return *reinterpret_cast<BrainInfo*>(_cell.brain); }

    BrainData AccessData()
    {
        const auto memory = std::span { _cell.brain + sizeof(BrainInfo), _cell.brain + Cell::brainSize };
        return BrainData { memory };
    }
};

class ConstBrain : public BrainViewBase<const Cell> {
public:
    ConstBrain(const Cell& cell)
        : BrainViewBase(cell)
    {
    }
};