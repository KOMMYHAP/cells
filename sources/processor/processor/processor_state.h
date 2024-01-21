#pragma once

enum class ProcessorState : unsigned char {
    Good,
    InvalidCommand,
    StackOverflow,
    StackUnderflow,
    StackCorrupted,
    MemoryCorrupted,
    UnknownProcedure,
};

using ProcessorStateWatcher = std::function<void(ProcessorState)>;
