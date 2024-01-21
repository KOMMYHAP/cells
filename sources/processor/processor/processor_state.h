#pragma once

enum class ProcessorState : unsigned char {
    Good,
    InvalidCommand,
    MemoryCorrupted,
    UnknownProcedure,
};
