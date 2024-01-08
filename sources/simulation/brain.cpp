#include "brain.h"
#include "brain_view.h"
#include "cell.h"

static_assert(Cell::brainSize >= sizeof(BrainInfo));

Brain::Brain(Cell& cell)
    : _cell(cell)
{
}

void Brain::Process()
{
    const auto memory = std::span { _cell.brain + sizeof(BrainInfo), _cell.brain + Cell::brainSize };
    assert(BrainData::memorySize == memory.size());

    const BrainData brainData {
        memory
    };

    switch (BrainView(_cell).GetType()) {
    case CellType::Unit:
        ProcessUnit(brainData);
        break;
    case CellType::Food:
        ProcessFood(brainData);
        break;
    case CellType::Wall:
        ProcessWall(brainData);
        break;
    case CellType::Dummy:
        break;
    }
}

void Brain::ProcessUnit(const BrainData& memory)
{
}

void Brain::ProcessFood(const BrainData& memory)
{
}

void Brain::ProcessWall(const BrainData& memory)
{
}
