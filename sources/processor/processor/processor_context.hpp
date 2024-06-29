#pragma once

template <class... Ts>
std::tuple<bool, Ts...> ProcessorContext::TryReadMemory()
{
    const auto result = _params.memory.TryRead<Ts...>();
    const bool success = std::get<0>(result);
    if (!success) {
        SetState(ProcessorState::OutOfMemory);
    }
    return result;
}
