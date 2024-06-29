#pragma once
#include "processor_external_context.h"

enum class ProcessorState : unsigned char {
    Good,
    InvalidInstruction,
    InvalidProcedure,
    ProcedureMissingInput,
    ProcedureTooMuchInputRequested,
    ProcedureIgnoreInput,
    ProcedureMissingOutput,
    ProcedureTooMuchOutput,
    StackOverflow,
    StackUnderflow,
    OutOfMemory,
    UnknownProcedure,
};

using ProcessorStateWatcher = std::function<void(ProcessorState, ProcessorExternalContext&)>;
