#include "spawn_system.h"

#include "components/procedure_type.h"
#include "procedures/direction.h"
#include "processor/processor_control_block.h"
#include "processor/processor_instruction.h"
#include "processor/processor_memory.h"
#include "systems/brain_system.h"
#include "systems/simulation_virtual_machine.h"

SpawnSystem::SpawnSystem(SimulationVirtualMachine& vm, BrainSystem& brainSystem)
    : _vm(vm)
    , _brainSystem(brainSystem)
{
}

bool SpawnSystem::MakePatrolUnit(CellId id, uint16_t length)
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

bool SpawnSystem::InitMemory(ProcessorMemory& memory)
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
