#include "processor_instruction.h"

ProcessorInstructionDescription GetProcessorInstructionDescription(ProcessorInstruction instruction)
{
    ProcessorInstructionDescription description;

    switch (instruction) {
    case ProcessorInstruction::WriteRegistryValue:
    case ProcessorInstruction::WriteRegistryRegistry:
    case ProcessorInstruction::CompareRegistryValue:
    case ProcessorInstruction::CompareRegistryRegistry:
    case ProcessorInstruction::AddRegistryValue:
    case ProcessorInstruction::AddRegistryRegistry:
    case ProcessorInstruction::SubtractRegistryValue:
    case ProcessorInstruction::SubtractRegistryRegistry:
        description.argumentsCount = 2;
        break;
    case ProcessorInstruction::Jump:
    case ProcessorInstruction::JumpIfEqual:
    case ProcessorInstruction::JumpIfNotEqual:
    case ProcessorInstruction::JumpIfLess:
    case ProcessorInstruction::JumpIfLessEqual:
    case ProcessorInstruction::JumpIfGreater:
    case ProcessorInstruction::JumpIfGreaterEqual:
    case ProcessorInstruction::Call:
    case ProcessorInstruction::PushStackRegistry:
    case ProcessorInstruction::PushStackValue:
    case ProcessorInstruction::PopStackRegistry:
        description.argumentsCount = 1;
        break;
    case ProcessorInstruction::Nope:
        description.argumentsCount = 0;
        break;
    case ProcessorInstruction::LastProcessorInstruction:
    default:
        ASSERT_FAIL("Unknown instruction!", instruction);
    }

    return description;
}