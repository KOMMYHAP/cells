#pragma once

#include "procedure_id.h"
#include "procedure_state.h"
#include "processor/processor_stack.h"
#include "processor/processor_user_data.h"

class ProcedureContext {
public:
    using State = ProcedureState;

    struct ArgumentsStatus {
        uint8_t input { 0 };
        uint8_t output { 0 };
    };

    // ProcedureContext() = default;
    ProcedureContext(ProcedureId id, ProcessorUserData userData, ProcessorStack stack, ArgumentsStatus arguments);

    template <MemoryType... Ts>
    std::tuple<bool, Ts...> TryPopArgs();

    template <class... Ts>
        requires(MemoryType<std::decay_t<Ts>> && ...)
    bool TryPushResult(Ts&&... ts);

    void AbortProcedure();

    const ProcessorUserData& GetUserData() const { return _userData; }
    ArgumentsStatus GetRestArgumentsCount() const { return _arguments; }
    ProcedureId GetId() const { return _id; }
    State GetState() const { return _state; }
    ProcessorStack GetStack() const { return _stack; }

    bool IsSucceeded() const;
    bool IsFailed() const;
    bool IsPending() const;

private:
    void SetState(State state);
    bool IsInitialState() const { return _state == State::Initial; }
    bool IsCompleted() const;

    ProcedureId _id { ProcedureId::Invalid };
    ArgumentsStatus _arguments;
    State _state { State::Initial };
    ProcessorStack _stack;
    ProcessorUserData _userData;
};

#include "procedure_context.hpp"