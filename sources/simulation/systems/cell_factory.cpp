#include "cell_factory.h"

#include "components/procedure_type.h"
#include "procedures/direction.h"
#include "processor/processor_control_block.h"
#include "processor/processor_instruction.h"
#include "processor/processor_memory.h"
#include "random.h"
#include "systems/brain_system.h"
#include "systems/simulation_virtual_machine.h"

CellFactory::CellFactory(SimulationVirtualMachine& vm, BrainSystem& brainSystem)
    : _vm(vm)
    , _brainSystem(brainSystem)
{
}

bool CellFactory::MakePatrolUnit(CellId id, uint16_t length)
{
    ProcessorMemory memory = _brainSystem.AccessMemory(id);
    if (!InitMemory(memory)) {
        return false;
    }

    const ProcedureId move = _vm.GetProcedureId(ProcedureType::Move);

    for (int i = 0; i < length; ++i) {
        memory.Write(ProcessorInstruction::PushStackValue, Direction::Right);
        memory.Write(ProcessorInstruction::Call, move);
    }
    for (int i = 0; i < length; ++i) {
        memory.Write(ProcessorInstruction::PushStackValue, Direction::Left);
        memory.Write(ProcessorInstruction::Call, move);
    }
    memory.Write(ProcessorInstruction::Jump, std::byte { 0 });

    return true;
}

bool CellFactory::MakeRandomUnit(CellId id, uint16_t lengthBytes)
{
    ProcessorMemory memory = _brainSystem.AccessMemory(id);
    if (!InitMemory(memory)) {
        return false;
    }
    if (lengthBytes > CellBrainLimit) {
        return false;
    }
    std::span<std::byte> rawBrain = memory.MakeSubSpan(0);
    std::byte* begin = rawBrain.data();
    std::byte* end = rawBrain.data() + rawBrain.size();
    std::byte* it = begin;

    {
        using Step = uint64_t;
        std::uniform_int_distribution<Step> distribution {};
        for (; it < end; it += sizeof(Step)) {
            const Step value = distribution(common::GetRandomEngine());
            void* destination = it;
            const void* source = &value;
            std::memcpy(destination, source, sizeof(Step));
        }
    }
    {
        using RandomUnit = uint16_t; // minimal available in uniform_int_distribution
        using Step = uint8_t;
        std::uniform_int_distribution<RandomUnit> distribution {};
        for (; it < end; ++it) {
            const Step value = distribution(common::GetRandomEngine()) % sizeof(Step);
            void* destination = it;
            const void* source = &value;
            std::memcpy(destination, source, sizeof(Step));
        }
    }

    return true;
}

bool CellFactory::InitMemory(ProcessorMemory& memory)
{
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