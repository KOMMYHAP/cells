#include "procedure_context.h"

namespace {

ProcessorStack MakeInvalidProcessorStack()
{
    // todo: remove this hack, add default ctor for ProcessorStack
    static uint8_t invalidStackOffsetStub;
    return ProcessorStack { {}, invalidStackOffsetStub };
}

}

ProcedureContext::ProcedureContext(ProcedureId id, ProcedureExternalContext externalContext, ProcessorStack stack, const ArgumentsStatus arguments)
    : _id(id)
    , _arguments(arguments)
    , _stack(std::move(stack))
    , _externalContext(std::move(externalContext))
{
}

void ProcedureContext::AbortProcedure()
{
    SetState(State::Aborted);
}

bool ProcedureContext::IsCompleted() const
{
    return _arguments.input == 0 && _arguments.output == 0;
}

bool ProcedureContext::IsSucceeded() const
{
    // We don't want to force user explicitly marks procedure as 'Succeeded' or 'Pending' to simplify its code flow.
    // Time will show if explicit is better than implicit.
    return IsCompleted() && _state == State::Initial;
}

bool ProcedureContext::IsFailed() const
{
    return IsCompleted() && _state != State::Initial;
}

bool ProcedureContext::IsPending() const
{
    return !IsCompleted() && _state == State::Initial;
}

void ProcedureContext::SetState(State state)
{
    ASSERT(state != State::Initial);
    _state = state;
}
