#include "processor.h"
#include "processor_profile_category.h"
#include "processor_state.h"

Processor::Processor(uint8_t systemInstructionToPerform)
    : _systemInstructionToPerform(systemInstructionToPerform)
{
    assert(_systemInstructionToPerform > 0);
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
    assert(context.IsState(ProcessorState::Good));

    ProcessorControlBlockGuard controlBlockGuard = context.MakeGuard();

    const auto [instructionRead, instruction] = context.TryReadMemory<ProcessorInstruction>();
    if (!instructionRead) {
        return {};
    }

    switch (instruction) {
    case ProcessorInstruction::Nope:
        if (!context.MoveCommandPointer(1)) {
            break;
        }
        controlBlockGuard.Submit();
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
        controlBlockGuard.Submit();
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
        controlBlockGuard.Submit();
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
        if (!context.WriteRegistry(registerIdx, static_cast<std::byte>(newValue))) {
            break;
        }
        if (!context.MoveCommandPointer(3)) {
            break;
        }
        controlBlockGuard.Submit();
    } break;
    case ProcessorInstruction::Jump: {
        const auto [success, commandPosition] = context.TryReadMemory<uint8_t>();
        if (!success) {
            break;
        }
        if (!context.SetCommandPointer(commandPosition)) {
            break;
        }
        controlBlockGuard.Submit();
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
        controlBlockGuard.Submit();
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
        controlBlockGuard.Submit();
    } break;
    case ProcessorInstruction::PushStack: {
        const auto [success, data] = context.TryReadMemory<std::byte>();
        if (!success) {
            break;
        }
        context.PushStack(data);
        if (!context.MoveCommandPointer(2)) {
            break;
        }
        controlBlockGuard.Submit();
    } break;
    case ProcessorInstruction::PopStack: {
        const auto [readSuccess, destinationRegistryIdx] = context.TryReadMemory<uint8_t>();
        if (!readSuccess) {
            break;
        }
        const auto [popSuccess, data] = context.PopStack();
        if (!popSuccess) {
            break;
        }
        if (!context.WriteRegistry(destinationRegistryIdx, data)) {
            break;
        }
        if (!context.MoveCommandPointer(2)) {
            break;
        }
        controlBlockGuard.Submit();
    } break;
    default:
        context.SetState(ProcessorState::InvalidInstruction);
        break;
    }

    if (controlBlockGuard.ShouldRollback()) {
        return {};
    }

    return instruction;
}