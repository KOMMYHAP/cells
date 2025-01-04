#include "random_cell_factory.h"

#include "procedures/procedure_id.h"
#include "processor/processor_control_block.h"
#include "processor/processor_instruction.h"
#include "processor/processor_memory.h"
#include "simulation/simulation_virtual_machine.h"

RandomCellFactory::RandomCellFactory(const SimulationVirtualMachine& vm, Random::Engine& random)
    : _vm(&vm)
    , _random(&random)
{
}

bool RandomCellFactory::Make(CellBrain& childBrainOut)
{
    ProcessorMemory memory { childBrainOut.data };
    if (!InitMemory(memory)) {
        return false;
    }

    while (true) {
        const ProcessorInstruction instruction = GenerateProcessorInstruction();
        const ProcessorInstructionDescription description = GetProcessorInstructionDescription(instruction);

        if (!memory.HasBytes(sizeof(instruction) + description.argumentsCount)) {
            while (memory.TryWrite(ProcessorInstruction::Nope)) { }
            break;
        }

        memory.Write(instruction);
        ASSERT(description.argumentsCount <= 2, "Invalid argument");
        if (description.argumentsCount == 2) {
            const auto [operand1, operand2] = GenerateTwoOperand(instruction);
            memory.Write(operand1, operand2);
        } else if (description.argumentsCount == 1) {
            const std::byte operand = GenerateOneOperand(instruction);
            memory.Write(operand);
        }
    }

    return true;
}

bool RandomCellFactory::InitMemory(ProcessorMemory& memory)
{
    ProcessorControlBlock controlBlock {
        static_cast<std::underlying_type_t<ProcessorState>>(ProcessorState::Good),
        static_cast<std::underlying_type_t<ProcedureId>>(ProcedureId::Invalid),
        0,
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
    const uint16_t value = distribution(*_random);
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
        ASSERT_FAIL("Unknown instruction!");
        return {};
    }
}

std::byte RandomCellFactory::GenerateRegistryIndex()
{
    std::uniform_int_distribution<uint16_t> distribution { 0, ProcessorRegistryCount - 1 };
    const auto value = NarrowCast<uint8_t>(distribution(*_random));
    return std::byte { value };
}

std::byte RandomCellFactory::GenerateValue()
{
    std::uniform_int_distribution<uint16_t> distribution { 0, ProcessorRegistryCount - 1 };
    const auto value = NarrowCast<uint8_t>(distribution(*_random));
    return std::byte { value };
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
        return GenerateProcedureId();
    default:
        ASSERT_FAIL("Unknown instruction!");
        return {};
    }
}

std::byte RandomCellFactory::GenerateProcedureId()
{
    std::uniform_int_distribution<uint16_t> distribution { 0, ProcedureCountLimit - 1 };
    const auto value = NarrowCast<uint8_t>(distribution(*_random));
    const auto procedureType = static_cast<ProcedureType>(value);
    const ProcedureId procedureId = _vm->GetProcedureId(procedureType);
    return std::byte { static_cast<uint8_t>(procedureId) };
}