#include "brain.h"
#include "cell.h"
#include "processor/brain_processor.h"
#include "processor/memory.h"

static_assert(Cell::brainSize >= sizeof(CellInfo));

Brain::Brain(Cell& cell)
    : BrainBase(cell)
{
}

CellInfo& Brain::AccessInfo() { return *reinterpret_cast<CellInfo*>(_cell.brain); }

Memory Brain::AccessMemory()
{
    const auto memory = std::span { _cell.brain + sizeof(CellInfo), _cell.brain + Cell::brainSize };
    return Memory { memory };
}

ConstBrain::ConstBrain(const Cell& cell)
    : BrainBase(cell)
{
}
