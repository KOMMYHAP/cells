#include "brain.h"
#include "brain_data.h"
#include "brain_view.h"
#include "cell.h"
#include "field.h"
#include "unit_processor.h"

static_assert(Cell::brainSize >= sizeof(BrainInfo));

Brain::Brain(Cell& cell)
    : _cell(cell)
{
    assert(Cell::brainSize - sizeof(BrainInfo) == AccessData().Size());
}

void Brain::Process(Field& field)
{
    switch (BrainView(_cell).GetType()) {
    case CellType::Unit:
        ProcessUnit(field);
        break;
    case CellType::Food:
        ProcessFood();
        break;
    case CellType::Wall:
        ProcessWall();
        break;
    case CellType::Dummy:
        break;
    }
}

void Brain::ProcessUnit(Field& field)
{
    Brain brain = _cell.GetBrain();
    UnitProcessor processor { brain, field };
    processor.Process();
}

void Brain::ProcessFood()
{
}

void Brain::ProcessWall()
{
}
