#pragma once

enum class ProcessorState : unsigned char {
    Good,
    InvalidCommand,
    StackOverflow,
    StackUnderflow,
    StackCorrupted,
    OutOfMemory,
    UnknownProcedure,
};

using ProcessorStateWatcher = std::function<void(ProcessorState)>;
