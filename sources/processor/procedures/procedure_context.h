#pragma once

#include "processor/processor_context.h"
#include "processor/processor_stack.h"
#include "processor/processor_state.h"

struct ProcedureTableEntry;
class ProcessorContext;
enum class ProcessorState : uint8_t;

class ProcedureContext {
public:
    struct ArgumentsStatus {
        uint8_t input { 0 };
        uint8_t output { 0 };
    };

    ProcedureContext(ProcedureId id, ProcessorContext context, ProcessorStack stack, ArgumentsStatus arguments);

    template <MemoryType... Ts>
    std::tuple<bool, Ts...> TryPopArgs();

    template <class... Ts>
        requires(MemoryType<std::decay_t<Ts>> && ...)
    bool TryPushResult(Ts&&... ts);

    void AbortProcedure();
    void DeferExecution();

    template <class T>
    T& ModifyExternalContext();

    template <class T>
    const T& GetExternalContext() const;

    ArgumentsStatus GetRestArgumentsCount() const { return _arguments; }
    ProcedureId GetId() const { return _id; }

private:
    void AbortProcedure(ProcessorState state);
    void CompleteProcedure();

    ProcedureId _id {ProcedureId::Invalid};
    ProcessorStack _stack;
    ProcessorContext _processorContext;
    ArgumentsStatus _arguments;
};

#include "procedure_context.hpp"