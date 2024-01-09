#include "brain.h"
#include "brain_data.h"
#include "cell.h"
#include "field.h"
#include "unit_processor.h"

static_assert(Cell::brainSize >= sizeof(BrainInfo));

