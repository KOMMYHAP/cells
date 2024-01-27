#pragma once
#include "processor_context.h"

class Processor {
public:
    Processor(uint8_t systemInstructionToPerform);

    void Execute(ProcessorContext& context);

private:
    std::optional<ProcessorInstruction> ProcessInstruction(ProcessorContext& context);

    uint8_t _systemInstructionToPerform;
};