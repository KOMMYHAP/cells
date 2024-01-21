#pragma once

template <MemoryType... Ts>
std::tuple<bool, Ts...> ProcedureContext::TryPopArgs()
{
    if (sizeof...(Ts) > _restInputArgs) {
        SetState(ProcessorState::InvalidCommand);
        return { false, Ts {}... };
    }

    _restInputArgs -= sizeof...(Ts);

    const auto result = _stack.TryPop<Ts...>();
    const bool success = std::get<0>(result);
    if (!success) {
        SetState(ProcessorState::StackUnderflow);
    }
    return result;
}

template <class... Ts>
    requires(MemoryType<std::decay_t<Ts>> && ...)
bool ProcedureContext::TryPushResult(Ts&&... ts)
{
    if (_restInputArgs != 0) {
        SetState(ProcessorState::InvalidCommand);
        return false;
    }
    if (sizeof...(Ts) > _restOutputArgs) {
        SetState(ProcessorState::InvalidCommand);
        return false;
    }
    _restOutputArgs -= sizeof...(Ts);

    const bool success = _stack.TryPush(std::forward<Ts>(ts)...);
    if (!success) {
        SetState(ProcessorState::StackOverflow);
    }
    return success;
}
