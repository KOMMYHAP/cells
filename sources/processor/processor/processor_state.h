#pragma once

enum class ProcessorState : unsigned char {
    Good,
    InvalidCommand,
    MemoryCorrupted,
    UnknownProcedure,
};

using ProcessorStateWatcher = std::function<void(ProcessorState)>;
