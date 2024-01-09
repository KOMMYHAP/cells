#pragma once

#include "brain_data.h"
#include "brain_view.h"

class Field;

class Brain {
public:
    Brain(Cell& cell);

    BrainInfo& AccessInfo() { return *reinterpret_cast<BrainInfo*>(_cell.brain); }
    const BrainInfo& GetInfo() const { return *reinterpret_cast<const BrainInfo*>(_cell.brain); }

    BrainData AccessData() { return GetData(); }
    BrainData GetData() const
    {
        const auto memory = std::span { _cell.brain + sizeof(BrainInfo), _cell.brain + Cell::brainSize };
        return BrainData { memory };
    }

private:
    Cell& _cell;
};