#include "procedure_context.h"

namespace {

// todo: remove this hack, add default ctor for ProcessorStack
uint8_t g_invalidStackOffsetStub;
const ProcessorStack g_invalidProcessorStack { {}, g_invalidStackOffsetStub };

}

ProcedureContext::ProcedureContext()
    : _stack(g_invalidProcessorStack)
{
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
    return _arguments.input == 0 && _arguments.output == 0 && _state == State::Initial;
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
