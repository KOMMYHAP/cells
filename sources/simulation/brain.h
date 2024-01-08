#pragma once

#include "brain_view.h"

class Brain {
public:
    Brain(Cell& cell);

    void Process();

    BrainInfo& AccessInfo() { return *reinterpret_cast<BrainInfo*>(_cell.brain); }
    CellType& AccessType() { return AccessInfo().type; }
    sf::Vector2<uint16_t>& AccessPosition() { return AccessInfo().position; }

private:
    void ProcessUnit(const BrainData& memory);
    void ProcessFood(const BrainData& memory);
    void ProcessWall(const BrainData& memory);

    Cell& _cell;
};