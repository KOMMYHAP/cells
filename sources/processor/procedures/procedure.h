#pragma once

#include "processor/memory.h"
#include "processor/processor_state.h"

struct ProcedureTableEntry;
class ProcessorContext;
enum class ProcessorState : uint8_t;

class ProcedureContext {
public:
    ProcedureContext(ProcessorContext& context, const Memory& memory);

    template <class... Ts>
    std::tuple<bool, Ts...> TryReadArgs();

    template <class... Ts>
    bool TryWriteResult(Ts&&... args);

    void NotifyInvalidCommand();

private:
    void SetState(ProcessorState state);

    Memory _memory;
    ProcessorContext& _processorContext;
};

class ProcedureBase {
public:
    virtual ~ProcedureBase() = default;
    virtual void Execute(ProcedureContext& context) = 0;
};

#include "procedure.hpp"
