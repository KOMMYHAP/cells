#pragma once

enum class ProcessorInstruction : uint8_t {
    Nope,
    Write,
    Compare,
    Add,
    Jump,
    JumpIfEqual,
    Call, ///< Procedure arguments are passed on the stack
    PushStack,
    PopStack,
    LastProcessorInstruction
};

constexpr uint8_t ProcessorInstructionCount = static_cast<uint8_t>(ProcessorInstruction::LastProcessorInstruction);