#pragma once

#include "procedure_external_context.h"
#include "procedure_id.h"
#include "processor/processor_stack.h"

class ProcedureContext {
public:
    struct ArgumentsStatus {
        uint8_t input { 0 };
        uint8_t output { 0 };
    };
    enum class State : uint8_t {
        Initial,
        Aborted,
        FailedMissingInput,
        FailedStackUnderflow,
        FailedTooMuchInputRequested,
        FailedIgnoreInput,
        FailedMissingOutput,
        FailedTooMuchOutput,
        FailedStackOverflow,
    };

    ProcedureContext();
    ProcedureContext(ProcedureId id, ProcedureExternalContext externalContext, ProcessorStack stack, ArgumentsStatus arguments);

    template <MemoryType... Ts>
    std::tuple<bool, Ts...> TryPopArgs();

    template <class... Ts>
        requires(MemoryType<std::decay_t<Ts>> && ...)
    bool TryPushResult(Ts&&... ts);

    void AbortProcedure();

    template <class T>
    T& ModifyExternalContext() { return _externalContext.Modify<T>(); }

    template <class T>
    const T& GetExternalContext() const { return _externalContext.Get<T>(); }

    ArgumentsStatus GetRestArgumentsCount() const { return _arguments; }
    ProcedureId GetId() const { return _id; }
    State GetState() const { return _state; }
    ProcessorStack GetStack() const { return _stack; }
    
    bool IsCompleted() const;
    bool IsPending() const;

private:
    void SetState(State state);
    bool IsInitialState() const { return _state == State::Initial; }

    ProcedureId _id { ProcedureId::Invalid };
    ArgumentsStatus _arguments;
    State _state { State::Initial };
    ProcessorStack _stack;
    ProcedureExternalContext _externalContext;
};

#include "procedure_context.hpp"