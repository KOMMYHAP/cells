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