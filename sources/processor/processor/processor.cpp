#include "processor.h"
#include "processor_instruction.h"
#include "processor_profile_category.h"
#include "processor_state.h"

Processor::Processor(uint8_t systemInstructionToPerform)
    : _systemInstructionToPerform(systemInstructionToPerform)
{
    DEBUG_ASSERT(_systemInstructionToPerform > 0);
}

void Processor::Execute(ProcessorContext& context)
{
    common::ProfileScope processorProfileScope { "Processor", ProcessorProfileCategory };

    for (int i = 0; i < _systemInstructionToPerform; ++i) {
        auto mbInstruction = ProcessInstruction(context);
        if (!mbInstruction.has_value()) {
            // no completed instruction
            break;
        }
        const ProcessorInstruction completedInstruction = *mbInstruction;
        if (completedInstruction == ProcessorInstruction::Call) {
            // one heavy instruction per execution is enough
            break;
        }
    }
}

std::optional<ProcessorInstruction> Processor::ProcessInstruction(ProcessorContext& context)
{
    ASSUME(context.IsState(ProcessorState::Good));

    ProcessorControlBlockGuard controlBlockGuard = context.MakeGuard();

    const auto [instructionRead, rawInstruction] = context.TryReadMemory<uint8_t>();
    if (!instructionRead) {
        return {};
    }
    const auto instruction = static_cast<ProcessorInstruction>(rawInstruction);

    switch (instruction) {
        /// instructions with two operands:
    case ProcessorInstruction::WriteRegistryRegistry:
    case ProcessorInstruction::CompareRegistryRegistry:
    case ProcessorInstruction::AddRegistryRegistry:
    case ProcessorInstruction::SubtractRegistryRegistry:
    case ProcessorInstruction::WriteRegistryValue:
    case ProcessorInstruction::CompareRegistryValue:
    case ProcessorInstruction::AddRegistryValue:
    case ProcessorInstruction::SubtractRegistryValue: {
        const auto [memoryReadSuccess, arg1, arg2] = context.TryReadMemory<uint8_t, uint8_t>();
        if (!memoryReadSuccess) {
            return {};
        }
        const TwoOperandsContext instructionContext {
            instruction,
            arg1,
            arg2,
        };
        if (!ProcessTwoOperands(instructionContext, context)) {
            return {};
        }
    } break;

        /// instructions with one operand:
    case ProcessorInstruction::PushStackRegistry:
    case ProcessorInstruction::PushStackValue:
    case ProcessorInstruction::PopStackRegistry:
    case ProcessorInstruction::Jump:
    case ProcessorInstruction::JumpIfEqual:
    case ProcessorInstruction::JumpIfNotEqual:
    case ProcessorInstruction::JumpIfLess:
    case ProcessorInstruction::JumpIfLessEqual:
    case ProcessorInstruction::JumpIfGreater:
    case ProcessorInstruction::JumpIfGreaterEqual:
    case ProcessorInstruction::Call: {
        const auto [memoryReadSuccess, arg] = context.TryReadMemory<uint8_t>();
        if (!memoryReadSuccess) {
            return {};
        }
        OneOperandContext instructionContext {
            instruction,
            arg,
        };
        if (!ProcessOneOperand(instructionContext, context)) {
            return {};
        }
    } break;

        /// instructions without operand:
    case ProcessorInstruction::Nope:
        ASSUME(GetProcessorInstructionDescription(instruction).argumentsCount == 0);
        if (!context.MoveCommandPointer(1)) {
            return {};
        }
        break;
    default:
    case ProcessorInstruction::LastProcessorInstruction:
        context.SetState(ProcessorState::InvalidInstruction);
        return {};
    }

    controlBlockGuard.Submit();
    return instruction;
}

bool Processor::ProcessTwoOperands(TwoOperandsContext instructionContext, ProcessorContext& context)
{
    ASSUME(GetProcessorInstructionDescription(instructionContext.instruction).argumentsCount == 2);

    /// Data extraction
    const uint8_t destinationIdx = instructionContext.operand1;
    uint8_t sourceData { 0 };
    const auto [destinationReadSuccess, destinationRawData] = context.ReadRegistry(instructionContext.operand1);
    if (!destinationReadSuccess) {
        return false;
    }
    const auto destinationData = static_cast<uint8_t>(destinationRawData);
    switch (instructionContext.instruction) {
    case ProcessorInstruction::WriteRegistryRegistry:
    case ProcessorInstruction::CompareRegistryRegistry:
    case ProcessorInstruction::AddRegistryRegistry:
    case ProcessorInstruction::SubtractRegistryRegistry: {
        const auto [sourceReadSuccess, sourceRegistryData] = context.ReadRegistry(instructionContext.operand2);
        if (!sourceReadSuccess) {
            return false;
        }
        sourceData = static_cast<uint8_t>(sourceRegistryData);
    } break;

    case ProcessorInstruction::WriteRegistryValue:
    case ProcessorInstruction::CompareRegistryValue:
    case ProcessorInstruction::AddRegistryValue:
    case ProcessorInstruction::SubtractRegistryValue: {
        const auto [sourceReadSuccess, sourceValueData] = context.TryReadMemory<uint8_t>();
        if (!sourceReadSuccess) {
            return false;
        }
        sourceData = sourceValueData;
    } break;
    default:
        break;
    }

    /// Data consumption
    uint8_t destinationNewData { 0 };
    switch (instructionContext.instruction) {
    case ProcessorInstruction::WriteRegistryValue:
    case ProcessorInstruction::WriteRegistryRegistry:
        destinationNewData = sourceData;
        break;
    case ProcessorInstruction::CompareRegistryValue:
    case ProcessorInstruction::CompareRegistryRegistry:
        destinationNewData = destinationData;
        break;
    case ProcessorInstruction::AddRegistryValue:
    case ProcessorInstruction::AddRegistryRegistry:
        destinationNewData = destinationData + sourceData;
        break;
    case ProcessorInstruction::SubtractRegistryValue:
    case ProcessorInstruction::SubtractRegistryRegistry: {
        destinationNewData = destinationData - sourceData;
    } break;
    default:
        break;
    }
    switch (instructionContext.instruction) {
    case ProcessorInstruction::CompareRegistryValue:
    case ProcessorInstruction::CompareRegistryRegistry:
    case ProcessorInstruction::AddRegistryValue:
    case ProcessorInstruction::AddRegistryRegistry:
    case ProcessorInstruction::SubtractRegistryValue:
    case ProcessorInstruction::SubtractRegistryRegistry: {
        const FlagsContext flagsContext {
            instructionContext.instruction,
            destinationData,
            sourceData
        };
        UpdateFlags(flagsContext, context);
    } break;
    default:
        break;
    }

    if (!context.WriteRegistry(destinationIdx, std::byte { destinationNewData })) {
        return false;
    }

    if (!context.MoveCommandPointer(1 /* instruction */ + 2 /* operand */)) {
        return false;
    }

    return true;
}

bool Processor::ProcessOneOperand(Processor::OneOperandContext instructionContext, ProcessorContext& context)
{
    ASSUME(GetProcessorInstructionDescription(instructionContext.instruction).argumentsCount == 1);

    /// Data extraction
    uint8_t sourceData { instructionContext.operand1 };

    switch (instructionContext.instruction) {
    case ProcessorInstruction::PushStackRegistry:
    case ProcessorInstruction::PopStackRegistry: {
        const auto [destinationReadSuccess, sourceRegistryData] = context.ReadRegistry(instructionContext.operand1);
        if (!destinationReadSuccess) {
            return false;
        }
        sourceData = static_cast<uint8_t>(sourceRegistryData);
    } break;
    default:
        break;
    }

    /// Data consumption
    switch (instructionContext.instruction) {
    case ProcessorInstruction::PushStackRegistry:
    case ProcessorInstruction::PushStackValue:
        if (!context.PushStack(std::byte { sourceData })) {
            return false;
        }
        break;
    case ProcessorInstruction::PopStackRegistry: {
        const auto [popSuccess, data] = context.PopStack();
        if (!popSuccess) {
            return false;
        }
        const uint8_t registryDestinationIdx = instructionContext.operand1;
        if (!context.WriteRegistry(registryDestinationIdx, data)) {
            return false;
        }
    } break;
    case ProcessorInstruction::Call: {
        const auto procedureIdx = static_cast<ProcedureId>(instructionContext.operand1);
        if (!context.RunProcedure(procedureIdx)) {
            return false;
        }
    } break;

        /// https://www.felixcloutier.com/x86/jcc
    case ProcessorInstruction::JumpIfEqual: {
        const bool shouldJump = context.HasFlag(ProcessorFlags::Zero);
        if (!shouldJump) {
            break;
        }
    }
    case ProcessorInstruction::JumpIfNotEqual: {
        const bool shouldJump = !context.HasFlag(ProcessorFlags::Zero);
        if (!shouldJump) {
            break;
        }
    }
    case ProcessorInstruction::JumpIfLess: {
        const bool shouldJump = context.HasFlag(ProcessorFlags::Sign) != context.HasFlag(ProcessorFlags::Overflow);
        if (!shouldJump) {
            break;
        }
    }
    case ProcessorInstruction::JumpIfLessEqual: {
        const bool shouldJump = context.HasFlag(ProcessorFlags::Zero) || context.HasFlag(ProcessorFlags::Sign) != context.HasFlag(ProcessorFlags::Overflow);
        if (!shouldJump) {
            break;
        }
    }
    case ProcessorInstruction::JumpIfGreater: {
        const bool shouldJump = context.HasFlag(ProcessorFlags::Zero) && context.HasFlag(ProcessorFlags::Sign) == context.HasFlag(ProcessorFlags::Overflow);
        if (!shouldJump) {
            break;
        }
    }
    case ProcessorInstruction::JumpIfGreaterEqual: {
        const bool shouldJump = context.HasFlag(ProcessorFlags::Sign) == context.HasFlag(ProcessorFlags::Overflow);
        if (!shouldJump) {
            break;
        }
    }
    case ProcessorInstruction::Jump: {
        const uint8_t commandPointer = instructionContext.operand1;
        return context.SetCommandPointer(commandPointer);
    }
    default:
        // Seems like a new instruction was added to ProcessInstruction, but wasn't processed here.
        UNREACHABLE("Unknown instruction", instructionContext.instruction);
    }

    return context.MoveCommandPointer(1 /* instruction */ + 1 /* operand */);
}

void Processor::UpdateFlags(FlagsContext flagsContext, ProcessorContext& context)
{
    int result { 0 };
    switch (flagsContext.instruction) {
    case ProcessorInstruction::CompareRegistryValue:
    case ProcessorInstruction::CompareRegistryRegistry:
    case ProcessorInstruction::SubtractRegistryValue:
    case ProcessorInstruction::SubtractRegistryRegistry:
        result = flagsContext.operand1 - flagsContext.operand2;
        break;
    case ProcessorInstruction::AddRegistryValue:
    case ProcessorInstruction::AddRegistryRegistry:
        result = flagsContext.operand1 + flagsContext.operand2;
        break;
    default:
        UNREACHABLE("Unknown instruction", flagsContext.instruction);
    }

    context.SetFlag(ProcessorFlags::Zero, result == 0);
    context.SetFlag(ProcessorFlags::Carry, result >= 0 && result <= 255);
    context.SetFlag(ProcessorFlags::Overflow, !(result >= -128 && result <= 127));
    context.SetFlag(ProcessorFlags::Sign, result > 0);
    context.SetFlag(ProcessorFlags::Parity, result % 2 == 0);
}
