#include "procedure_context.h"
#include "processor/processor.h"

ProcedureContext::ProcedureContext(ProcedureId id, ProcessorContext context, ProcessorStack stack, const ArgumentsStatus arguments)
    : _id(id)
    , _stack(std::move(stack))
    , _processorContext(std::move(context))
    , _arguments(arguments)
{
}

void ProcedureContext::AbortProcedure()
{
    AbortProcedure(ProcessorState::AbortedProcedure);
}

void ProcedureContext::DeferExecution()
{
    _processorContext.DeferProcedure(*this);
}

void ProcedureContext::AbortProcedure(const ProcessorState state)
{
    _processorContext.AbortProcedure(*this, state);
}

void ProcedureContext::CompleteProcedure()
{
    _processorContext.CompleteProcedure(*this);
}
