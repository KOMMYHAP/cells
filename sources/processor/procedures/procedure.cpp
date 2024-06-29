#include "procedure.h"
#include "processor/processor.h"

ProcedureContext::ProcedureContext(ProcessorContext& context, ProcessorStack stack, uint8_t& inputArgs, uint8_t& outputArgs)
    : _stack(stack)
    , _processorContext(&context)
    , _restInputArgs(&inputArgs)
    , _restOutputArgs(&outputArgs)
{
}

void ProcedureContext::SetState(ProcessorState state)
{
    _processorContext->SetState(state);
}

void ProcedureContext::MarkProcedureAsInvalid()
{
    SetState(ProcessorState::InvalidProcedure);
}

const ProcessorExternalContext& ProcedureContext::GetExternalContext() const
{
    return _processorContext->GetExternalContext();
}

ProcessorExternalContext& ProcedureContext::ModifyExternalContext()
{
    return _processorContext->ModifyExternalContext();
}