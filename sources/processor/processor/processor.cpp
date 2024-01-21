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
    case ProcessorInstruction::Compare:
        break;
    case ProcessorInstruction::Decrement:
        break;
    case ProcessorInstruction::Increment:
        break;
    case ProcessorInstruction::Jump:
        break;
    case ProcessorInstruction::JumpIfEqual:
        break;
    case ProcessorInstruction::Call:
        break;
    default:
        assert(false);
        context.SetState(ProcessorState::InvalidCommand);
        break;
    }
}
