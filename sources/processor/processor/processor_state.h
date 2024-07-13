#pragma once

enum class ProcessorState : uint8_t {
    Good,
    InvalidInstruction,
    PendingProcedure,
    AbortedProcedure,
    StackOverflow,
    StackUnderflow,
    OutOfMemory,
    UnknownProcedure,
};
