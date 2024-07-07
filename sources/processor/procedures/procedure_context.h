#pragma once

#include "processor/processor_context.h"
#include "processor/processor_stack.h"
#include "processor/processor_state.h"

struct ProcedureTableEntry;
class ProcessorContext;
enum class ProcessorState : uint8_t;

class ProcedureContext {
public:
    ProcedureContext(ProcedureId id, ProcessorContext context, ProcessorStack stack, uint8_t inputArgs, uint8_t outputArgs);

    template <MemoryType... Ts>
    std::tuple<bool, Ts...> TryPopArgs();

    template <class... Ts>
        requires(MemoryType<std::decay_t<Ts>> && ...)
    bool TryPushResult(Ts&&... ts);

    void AbortProcedure();

    template <class T>
    T& ModifyExternalContext();

    template <class T>
    const T& GetExternalContext() const;

private:
    void AbortProcedure(ProcessorState state);
    void CompleteProcedure();

    ProcedureId _id;
    ProcessorStack _stack;
    ProcessorContext _processorContext;
    uint8_t _restInputArgs { 0 };
    uint8_t _restOutputArgs { 0 };
};

#include "procedure_context.hpp"