#include "procedure.h"
#include "processor/processor.h"

ProcedureContext::ProcedureContext(ProcessorContext& context, const Memory& memory)
    : _processorContext(context)
    , _memory(memory)
{
}

void ProcedureContext::SetState(ProcessorState state)
{
    _processorContext.SetState(state);
}