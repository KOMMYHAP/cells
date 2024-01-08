#include "brain_view.h"

BrainView::BrainView(const Cell& cell)
    : _cell(cell)
{
}

const BrainInfo& BrainView::GetBrainInfo() const
{
    return *reinterpret_cast<const BrainInfo*>(_cell.brain);
}