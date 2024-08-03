#pragma once

enum class ProcedureState : uint8_t {
    Initial,
    Aborted,
    FailedMissingInput,
    FailedTooMuchInputRequested,
    FailedIgnoreInput,
    FailedMissingOutput,
    FailedTooMuchOutput,

    // todo: remove this when Procedure will reuse ProcessorContext
    FailedStackUnderflow,
    FailedStackOverflow,
};