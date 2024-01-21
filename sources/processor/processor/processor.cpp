#include "processor.h"
#include "processor_profile_category.h"

void Processor::Execute(ProcessorContext& context)
{
    common::ProfileScope processorProfileScope { "Processor", ProcessorProfileCategory };

    ProcessInstruction(context);
}

void Processor::ProcessInstruction(ProcessorContext& context)
{
    if (context.IsState(ProcessorState::InvalidCommand)) {
        return;
    }

    const auto [instructionRead, instruction] = context.TryReadMemory<ProcessorInstruction>();
    if (!instructionRead) {
        return;
    }

    switch (instruction) {
    case ProcessorInstruction::Nope:
        break;
    case ProcessorInstruction::Write: {
        const auto [success, registerIdx, data] = context.TryReadMemory<uint8_t, std::byte>();
        if (!success) {
            break;
        }
        if (!context.WriteRegistry(registerIdx, data)) {
            break;
        }
        if (!context.MoveCommandPointer(3)) {
            break;
        }
    } break;
    case ProcessorInstruction::Compare: {
        const auto [success, registerIdx, valueToTest] = context.TryReadMemory<uint8_t, std::byte>();
        if (!success) {
            break;
        }
        const auto [registerRead, registerData] = context.ReadRegistry(registerIdx);
        if (!registerRead) {
            break;
        }
        const ProcessorFlags compareFlag = registerData == valueToTest ? ProcessorFlags::Equal : registerData < valueToTest ? ProcessorFlags::Less
                                                                                                                            : ProcessorFlags::Greater;
        context.SetFlag(compareFlag);
        if (!context.MoveCommandPointer(3)) {
            break;
        }
    } break;
    case ProcessorInstruction::Add: {
        const auto [success, registerIdx, value] = context.TryReadMemory<uint8_t, uint8_t>();
        if (!success) {
            break;
        }
        const auto [registerRead, registerData] = context.ReadRegistry(registerIdx);
        if (!registerRead) {
            break;
        }
        const uint8_t newValue = static_cast<uint8_t>(registerData) + value;
        [[maybe_unused]] const bool registerWrite = context.WriteRegistry(registerIdx, static_cast<std::byte>(newValue));
        assert(registerWrite);
        if (!context.MoveCommandPointer(3)) {
            break;
        }
    } break;
    case ProcessorInstruction::Jump: {
        const auto [success, commandPosition] = context.TryReadMemory<uint8_t>();
        if (!success) {
            break;
        }
        if (!context.SetCommandPointer(commandPosition)) {
            break;
        }
    } break;
    case ProcessorInstruction::JumpIfEqual: {
        const auto [success, commandPosition] = context.TryReadMemory<uint8_t>();
        if (!success) {
            break;
        }
        if (context.HasFlag(ProcessorFlags::Equal)) {
            if (!context.SetCommandPointer(commandPosition)) {
                break;
            }
        } else {
            if (!context.MoveCommandPointer(2)) {
                break;
            }
        }
    } break;
    case ProcessorInstruction::Call: {
        const auto [success, procedureIdx] = context.TryReadMemory<ProcedureId>();
        if (!success) {
            break;
        }
        if (!context.RunProcedure(procedureIdx)) {
            break;
        }
        if (!context.MoveCommandPointer(2)) {
            break;
        }
    } break;
    default:
        assert(false);
        context.SetState(ProcessorState::InvalidCommand);
        break;
    }
}
