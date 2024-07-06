#pragma once
#include "processor_external_context.h"

enum class ProcessorState : uint8_t {
    Good,
    InvalidInstruction,
    InvalidProcedure,
    ProcedureMissingInput,
    ProcedureTooMuchInputRequested,
    ProcedureIgnoreInput,
    ProcedureMissingOutput,
    ProcedureTooMuchOutput,
    IncompleteDelayedProcedure,
    UnknownDelayedProcedure,
    StackOverflow,
    StackUnderflow,
    OutOfMemory,
    UnknownProcedure,
};

using ProcessorStateWatcher = std::function<void(ProcessorState, ProcessorExternalContext&)>;
