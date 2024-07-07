#pragma once

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
