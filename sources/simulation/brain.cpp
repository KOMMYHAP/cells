#include "brain.h"
#include "cell.h"

static_assert(sizeof(Cell::brainSize) >= sizeof(BrainInfo));

Brain::Brain(Cell& cell)
    : _cell(cell)
{
}

CellType Brain::GetType() const
{
    return GetBrainInfo().type;
}

BrainInfo& Brain::GetBrainInfo()
{
    return *reinterpret_cast<BrainInfo*>(_cell.brain);
}

const BrainInfo& Brain::GetBrainInfo() const
{
    return *reinterpret_cast<const BrainInfo*>(_cell.brain);
}

void Brain::Process()
{
    const auto memory = std::span { _cell.brain + sizeof(BrainInfo), _cell.brain + Cell::brainSize };
    assert(BrainData::memorySize == memory.size());

    const BrainData brainData {
        memory
    };

    switch (GetType()) {
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
