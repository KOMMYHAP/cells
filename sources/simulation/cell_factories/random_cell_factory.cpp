#include "random_cell_factory.h"

#include "procedures/procedure_id.h"
#include "processor/processor_control_block.h"
#include "processor/processor_instruction.h"
#include "processor/processor_memory.h"
#include "random.h"
#include "systems/simulation_virtual_machine.h"

RandomCellFactory::RandomCellFactory(SimulationVirtualMachine& vm, std::optional<uint16_t> limitBrainSize /* = {}*/)
    : _brainSize(limitBrainSize.value_or(CellBrainLimit - sizeof(ProcessorControlBlock)))
    , _vm(vm)
{
}

auto RandomCellFactory::Make() -> Result
{
    Result result;
    result.status = Status::FailedToCreate;

    ProcessorMemory memory { result.brain.data };
    if (!InitMemory(memory)) {
        return result;
    }
    if (!memory.HasBytes(_brainSize)) {
        return result;
    }

    while (true) {
        const ProcessorInstruction instruction = GenerateProcessorInstruction();
        const ProcessorInstructionDescription description = GetProcessorInstructionDescription(instruction);

        if (!memory.HasBytes(sizeof(instruction) + description.argumentsCount)) {
            while (memory.TryWrite(ProcessorInstruction::Nope)) {
            }
            break;
        }

        memory.Write(instruction);
        ASSERT(description.argumentsCount <= 2);
        if (description.argumentsCount == 2) {
            const auto [operand1, operand2] = GenerateTwoOperand(instruction);
            memory.Write(operand1, operand2);
        } else if (description.argumentsCount == 1) {
            const std::byte operand = GenerateOneOperand(instruction);
            memory.Write(operand);
        }
    }

    result.status = Status::Success;
    return result;
}

bool RandomCellFactory::InitMemory(ProcessorMemory& memory)
{
    // todo: extract it
    ProcessorControlBlock controlBlock {
        static_cast<uint8_t>(ProcessorState::Good),
        static_cast<uint8_t>(0),
        0,
        {},
        0,
        {}
    };
    return memory.TryWrite(controlBlock);
}

ProcessorInstruction RandomCellFactory::GenerateProcessorInstruction()
{
    std::uniform_int_distribution<uint16_t> distribution { 0, ProcessorInstructionCount - 1 };
    const uint16_t value = distribution(common::GetRandomEngine());
    return static_cast<ProcessorInstruction>(value);
}

std::tuple<std::byte, std::byte> RandomCellFactory::GenerateTwoOperand(ProcessorInstruction hint)
{
    switch (hint) {
    case ProcessorInstruction::WriteRegistryRegistry:
    case ProcessorInstruction::CompareRegistryRegistry:
    case ProcessorInstruction::AddRegistryRegistry:
    case ProcessorInstruction::SubtractRegistryRegistry:
        return { GenerateRegistryIndex(), GenerateRegistryIndex() };
    case ProcessorInstruction::WriteRegistryValue:
    case ProcessorInstruction::CompareRegistryValue:
    case ProcessorInstruction::AddRegistryValue:
    case ProcessorInstruction::SubtractRegistryValue:
        return { GenerateRegistryIndex(), GenerateValue() };
    default:
        UNREACHABLE("Unknown instruction!", hint);
    }
}

std::byte RandomCellFactory::GenerateRegistryIndex()
{
    std::uniform_int_distribution<uint16_t> distribution { 0, ProcessorRegistryCount - 1 };
    const uint8_t value = distribution(common::GetRandomEngine());
    return static_cast<std::byte>(value);
}

std::byte RandomCellFactory::GenerateValue()
{
    std::uniform_int_distribution<uint16_t> distribution { 0, ProcessorRegistryCount - 1 };
    const uint8_t value = distribution(common::GetRandomEngine());
    return static_cast<std::byte>(value);
}

std::byte RandomCellFactory::GenerateOneOperand(ProcessorInstruction hint)
{
    switch (hint) {
    case ProcessorInstruction::Jump:
    case ProcessorInstruction::JumpIfEqual:
    case ProcessorInstruction::JumpIfNotEqual:
    case ProcessorInstruction::JumpIfLess:
    case ProcessorInstruction::JumpIfLessEqual:
    case ProcessorInstruction::JumpIfGreater:
    case ProcessorInstruction::JumpIfGreaterEqual:
    case ProcessorInstruction::PushStackValue:
        return GenerateValue();
    case ProcessorInstruction::PushStackRegistry:
    case ProcessorInstruction::PopStackRegistry:
        return GenerateRegistryIndex();
    case ProcessorInstruction::Call:
        return static_cast<std::byte>(GenerateProcedureId());
    default:
        UNREACHABLE("Unknown instruction!", hint);
    }
}

std::byte RandomCellFactory::GenerateProcedureId()
{
    std::uniform_int_distribution<uint16_t> distribution { 0, ProcedureCountLimit - 1 };
    const uint8_t value = distribution(common::GetRandomEngine());
    const ProcedureType procedureType = static_cast<ProcedureType>(value);
    const ProcedureId procedureId = _vm.GetProcedureId(procedureType);
    return static_cast<std::byte>(procedureId);
}
