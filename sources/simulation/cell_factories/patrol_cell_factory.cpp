#include "patrol_cell_factory.h"

#include "components/procedure_type.h"
#include "procedures/direction.h"
#include "processor/processor_control_block.h"
#include "processor/processor_instruction.h"
#include "processor/processor_memory.h"
#include "systems/brain_system.h"
#include "systems/simulation_virtual_machine.h"

PatrolCellFactory::PatrolCellFactory(SimulationVirtualMachine& vm, uint8_t moveCommandCount)
    : _moveCommandCount(moveCommandCount)
    , _simulationVm(vm)
{
}

bool PatrolCellFactory::InitMemory(ProcessorMemory& memory)
{
    ProcessorControlBlock controlBlock {
        static_cast<uint8_t>(ProcessorState::Good),
        0,
        0,
        {},
        0,
        {}
    };
    return memory.TryWrite(controlBlock);
}

PatrolCellFactory::Result PatrolCellFactory::Make()
{
    Result result;
    result.status = Status::FailedToCreate;

    ProcessorMemory memory { result.brain.data };
    if (!InitMemory(memory)) {
        return result;
    }

    const ProcedureId move = _simulationVm.GetProcedureId(ProcedureType::Move);

    for (uint8_t i = 0; i < _moveCommandCount; ++i) {
        memory.Write(ProcessorInstruction::PushStackValue, Direction::Right);
        memory.Write(ProcessorInstruction::Call, move);
    }
    for (uint8_t i = 0; i < _moveCommandCount; ++i) {
        memory.Write(ProcessorInstruction::PushStackValue, Direction::Left);
        memory.Write(ProcessorInstruction::Call, move);
    }
    memory.Write(ProcessorInstruction::Jump, std::byte { 0 });

    result.status = Status::Success;
    return result;
}
