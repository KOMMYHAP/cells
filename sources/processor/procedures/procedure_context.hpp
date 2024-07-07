#pragma once

template <MemoryType... Ts>
std::tuple<bool, Ts...> ProcedureContext::TryPopArgs()
{
    if (!_processorContext.IsState(ProcessorState::Good)) {
        return { false, Ts {}... };
    }

    if (sizeof...(Ts) > _arguments.input) {
        AbortProcedure(ProcessorState::ProcedureTooMuchInputRequested);
        return { false, Ts {}... };
    }

    _arguments.input -= sizeof...(Ts);

    const auto result = _stack.TryPop<Ts...>();
    const bool success = std::get<0>(result);
    if (!success) {
        AbortProcedure(ProcessorState::StackUnderflow);
    }
    return result;
}

template <class... Ts>
    requires(MemoryType<std::decay_t<Ts>> && ...)
bool ProcedureContext::TryPushResult(Ts&&... ts)
{
    if (!_processorContext.IsState(ProcessorState::Good)) {
        return false;
    }
    if (_arguments.input != 0) {
        AbortProcedure(ProcessorState::ProcedureIgnoreInput);
        return false;
    }
    if (sizeof...(Ts) > _arguments.output) {
        AbortProcedure(ProcessorState::ProcedureTooMuchOutput);
        return false;
    }
    _arguments.output -= sizeof...(Ts);

    const bool success = _stack.TryPush(std::forward<Ts>(ts)...);
    if (!success) {
        AbortProcedure(ProcessorState::StackOverflow);
        return false;
    }

    if (_arguments.output == 0) {
        CompleteProcedure();
    }
    return success;
}

template <class T>
T& ProcedureContext::ModifyExternalContext()
{
    return _processorContext.GetExternalContext().Modify<T>();
}

template <class T>
const T& ProcedureContext::GetExternalContext() const
{
    return _processorContext.GetExternalContext().Get<T>();
}
