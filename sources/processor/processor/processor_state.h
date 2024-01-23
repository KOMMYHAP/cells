#pragma once

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

using ProcessorStateWatcher = std::function<void(ProcessorState)>;
