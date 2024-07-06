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

void ProcedureContext::MarkProcedureAsInvalid()
{
    SetState(ProcessorState::InvalidProcedure);
}

const ProcessorExternalContext& ProcedureContext::GetExternalContext() const
{
    return _processorContext.GetExternalContext();
}

ProcessorExternalContext& ProcedureContext::ModifyExternalContext()
{
    return _processorContext.ModifyExternalContext();
}

void ProcedureContext::SetState(const ProcessorState state)
{
    _processorContext.SetState(state);
    _processorContext.CompleteProcedure(_id, _restInputArgs, _restOutputArgs);
}
