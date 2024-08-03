#pragma once
#include "procedure_context.h"

template <MemoryType... Ts>
std::tuple<bool, Ts...> ProcedureContext::TryPopArgs()
{
    if (!IsInitialState()) {
        return { false, Ts {}... };
    }

    if (sizeof...(Ts) > _arguments.input) {
        SetState(State::FailedTooMuchInputRequested);
        return { false, Ts {}... };
    }

    _arguments.input -= sizeof...(Ts);

    const auto result = _stack.TryPop<Ts...>();
    const bool success = std::get<0>(result);
    if (!success) {
        SetState(State::FailedStackUnderflow);
    }
    return result;
}

template <class... Ts>
    requires(MemoryType<std::decay_t<Ts>> && ...)
bool ProcedureContext::TryPushResult(Ts&&... ts)
{
    if (!IsInitialState()) {
        return false;
    }
    if (_arguments.input != 0) {
        SetState(State::FailedIgnoreInput);
        return false;
    }

    // todo:
    // TryPush(a), TryPush(b), ... do we need it?
    // Shall we do only TryPush(a, b) at once?
    // See the same case in TryPopArgs.
    if (sizeof...(Ts) > _arguments.output) {
        SetState(State::FailedTooMuchOutput);
        return false;
    }
    _arguments.output -= sizeof...(Ts);

    const bool success = _stack.TryPush(std::forward<Ts>(ts)...);
    if (!success) {
        SetState(State::FailedStackOverflow);
        return false;
    }

    return success;
}