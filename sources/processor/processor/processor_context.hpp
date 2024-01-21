#pragma once

template <class... Ts>
std::tuple<bool, Ts...> ProcessorContext::TryReadMemory()
{
    const auto result = AccessMemory().TryRead<Ts...>();
    const bool success = std::get<0>(result);
    if (!success) {
        SetState(ProcessorState::MemoryCorrupted);
    }
    return result;
}
