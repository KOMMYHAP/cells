#pragma once

#include "brain_packed_data.h"
#include "cell.h"
#include "memory.h"

namespace Details {

template <class Unit>
class BrainBase {
public:
    BrainBase(Unit& cell);

    const CellInfo& GetInfo() const;
    ConstMemory GetView() const;

protected:
    Unit& _cell;
};

}

class Brain : public Details::BrainBase<Cell> {
public:
    Brain(Cell& cell);

    CellInfo& AccessInfo();
    Memory Access();
};

class ConstBrain : public Details::BrainBase<const Cell> {
public:
    ConstBrain(const Cell& cell);
};

#include "brain.hpp"