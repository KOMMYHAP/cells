#pragma once

#include "processor/memory.h"
#include "processor/processor_control_block.h"

struct ProcedureInfo;
class ProcessorContext;

class ProcedureContext {
public:
    ProcedureContext(ProcessorContext& context, const Memory& memory);

    template <class... Ts>
    std::tuple<bool, Ts...> TryReadArgs()
    {
        const auto result = _memory.TryRead<Ts...>();
        const bool success = std::get<0>(result);
        if (!success) {
            SetState(ProcessorState::MemoryCorrupted);
        }
        return result;
    }

    template <class... Ts>
    bool TryWriteResult(Ts&&... args)
    {
        const bool success = _memory.TryWrite(std::forward<Ts>(args)...);
        if (!success) {
            SetState(ProcessorState::MemoryCorrupted);
        }
        return success;
    }

private:
    void SetState(ProcessorState state);

    Memory _memory;
    ProcessorContext& _processorContext;
};

struct ProcedureBase {
    virtual void Execute(ProcedureContext& context) = 0;
};