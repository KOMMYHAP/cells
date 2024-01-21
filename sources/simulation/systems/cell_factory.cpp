#include "cell_factory.h"
#include "components/procedure_type.h"
#include "procedures/move_procedure.h"
#include "processor/processor_control_block.h"
#include "processor/processor_memory.h"

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
        memory.Write(move, MoveDirection::Right);
    }
    for (int i = 0; i < length; ++i) {
        memory.Write(move, MoveDirection::Left);
    }
    memory.Write(ProcessorInstruction::Jump, std::byte { 0 });

    return true;
}

bool CellFactory::InitMemory(ProcessorMemory& memory)
{
    ProcessorControlBlock controlBlock {
        static_cast<uint8_t>(ProcessorState::Good),
        static_cast<uint8_t>(ProcessorFlags ::None),
        0,
        {}
    };
    return memory.TryWrite(controlBlock);
}
