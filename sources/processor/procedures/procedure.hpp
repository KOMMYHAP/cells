#pragma once

template <class... Ts>
bool ProcedureContext::TryWriteResult(Ts&&... args)
{
    const bool success = _memory.TryWrite(std::forward<Ts>(args)...);
    if (!success) {
        SetState(ProcessorState::MemoryCorrupted);
    }
    return success;
}

template <class... Ts>
std::tuple<bool, Ts...> ProcedureContext::TryReadArgs()
{
    const auto result = _memory.TryRead<Ts...>();
    const bool success = std::get<0>(result);
    if (!success) {
        SetState(ProcessorState::MemoryCorrupted);
    }
    return result;
}
