#include "brain.h"
#include "brain_processor.h"
#include "cell.h"
#include "processor/memory.h"

Brain::Brain(Cell& cell)
    : BrainBase(cell)
{
}

Memory Brain::AccessMemory()
{
    const auto memory = std::span { _cell.brain, _cell.brain + Cell::brainSize };
    return Memory { memory };
}

ConstBrain::ConstBrain(const Cell& cell)
    : BrainBase(cell)
{
}
