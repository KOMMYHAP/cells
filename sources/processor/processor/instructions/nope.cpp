#include "nope.h"

namespace processor::instructions {

bool Nope::Execute(ProcessorContext& context)
{
    return false;
}
}