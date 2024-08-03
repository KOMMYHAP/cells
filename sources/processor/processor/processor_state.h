#pragma once

enum class ProcessorState : uint8_t {
    Good,
    InvalidInstruction,
    UnknownProcedure,
    PendingProcedure,
    AbortedProcedure,
    StackOverflow,
    StackUnderflow,
    OutOfMemory,
};
