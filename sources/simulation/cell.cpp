#include "cell.h"
#include "brain.h"
#include "brain_view.h"

Brain Cell::GetBrain()
{
    return Brain(*this);
}

BrainView Cell::GetBrain() const
{
    return BrainView(*this);
}