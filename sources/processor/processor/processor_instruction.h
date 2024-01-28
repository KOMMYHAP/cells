#pragma once

enum class ProcessorInstruction : uint8_t {
    Nope,
    WriteRegistryValue, // write value to specified registry
    WriteRegistryRegistry, // write value to specified registry
    CompareRegistryRegistry,
    CompareRegistryValue,
    AddRegistryValue,
    AddRegistryRegistry,
    SubtractRegistryValue,
    SubtractRegistryRegistry,
    Jump,
    JumpIfEqual,
    JumpIfNotEqual,
    JumpIfLess,
    JumpIfLessEqual,
    JumpIfGreater,
    JumpIfGreaterEqual,
    Call,
    PushStackRegistry,
    PushStackValue,
    PopStackRegistry,
    LastProcessorInstruction
};

constexpr uint8_t ProcessorInstructionCount = static_cast<uint8_t>(ProcessorInstruction::LastProcessorInstruction);

