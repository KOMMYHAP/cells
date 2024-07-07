#include "procedure_context.h"
#include "processor/processor.h"

ProcedureContext::ProcedureContext(ProcedureId id, ProcessorContext context, ProcessorStack stack, const uint8_t inputArgs, const uint8_t outputArgs)
    : _id(id)
    , _stack(std::move(stack))
    , _processorContext(std::move(context))
    , _restInputArgs(inputArgs)
    , _restOutputArgs(outputArgs)
{
}

void ProcedureContext::AbortProcedure()
{
    AbortProcedure(ProcessorState::AbortedProcedure);
}

void ProcedureContext::AbortProcedure(const ProcessorState state)
{
    _processorContext.AbortProcedure(_id, state);
}

void ProcedureContext::CompleteProcedure()
{
    _processorContext.CompleteProcedure(_id, _restInputArgs, _restOutputArgs);
}
