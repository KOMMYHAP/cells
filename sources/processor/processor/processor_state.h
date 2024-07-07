#pragma once
#include "processor_external_context.h"

enum class ProcessorState : uint8_t {
    Good,
    InvalidInstruction,
    AbortedProcedure,
    ProcedureMissingInput,
    ProcedureTooMuchInputRequested,
    ProcedureIgnoreInput,
    ProcedureMissingOutput,
    ProcedureTooMuchOutput,
    IncompletePendingProcedure,
    UnknownPendingProcedure,
    StackOverflow,
    StackUnderflow,
    OutOfMemory,
    UnknownProcedure,
};

using ProcessorStateWatcher = std::function<void(ProcessorState, ProcessorExternalContext&)>;
