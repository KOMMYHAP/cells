#pragma once

template <class... Ts>
std::tuple<bool, Ts...> ProcessorContext::TryReadMemory()
{
    ConstMemory memory = GetMemory();
    if (!memory.HasBytes<Ts...>()) {
        SetState(ProcessorState::MemoryCorrupted);
        return { false, Ts {}... };
    }
    return { true, memory.Read<Ts>()... };
}
