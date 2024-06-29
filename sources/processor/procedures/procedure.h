#pragma once

#include "processor/processor_stack.h"
#include "processor/processor_state.h"

struct ProcedureTableEntry;
class ProcessorContext;
enum class ProcessorState : uint8_t;

class ProcedureContext {
public:
    ProcedureContext(ProcessorContext& context, ProcessorStack stack, uint8_t& inputArgs, uint8_t& outputArgs);

    template <MemoryType... Ts>
    std::tuple<bool, Ts...> TryPopArgs();

    template <class... Ts>
        requires(MemoryType<std::decay_t<Ts>> && ...)
    bool TryPushResult(Ts&&... ts);

    void MarkProcedureAsInvalid();

    const ProcessorExternalContext& GetExternalContext() const;
    ProcessorExternalContext& ModifyExternalContext();

private:
    void SetState(ProcessorState state);

    ProcessorStack _stack;
    gsl::not_null<ProcessorContext*> _processorContext;
    gsl::not_null<uint8_t*> _restInputArgs;
    gsl::not_null<uint8_t*> _restOutputArgs;
};

class ProcedureBase {
public:
    virtual ~ProcedureBase() = default;
    virtual void Execute(ProcedureContext& context) = 0;
};

#include "procedure.hpp"
