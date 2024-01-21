#pragma once

template <class... Ts>
std::tuple<bool, Ts...> ProcessorContext::TryReadMemory()
{
    const auto result = GetMemory().TryRead<Ts...>();
    const bool success = std::get<0>(result);
    if (!success) {
        SetState(ProcessorState::MemoryCorrupted);
    }
    return result;
}
