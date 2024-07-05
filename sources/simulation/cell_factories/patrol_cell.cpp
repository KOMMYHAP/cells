#include "patrol_cell.h"

#include "components/direction.h"
#include "processor/processor_control_block.h"
#include "processor/processor_instruction.h"

CellBrain MakePatrolCell(const SimulationVirtualMachine& vm)
{
    CellBrain brain;
    ProcessorMemory memory { brain.data };
    ProcessorControlBlock controlBlock {
        static_cast<uint8_t>(ProcessorState::Good),
        0,
        0,
        {},
        0,
        {}
    };
    memory.Write(controlBlock);

    const ProcedureId move = vm.GetProcedureId(ProcedureType::Move);

    constexpr int moveCommandCount { 10 };
    for (int i = 0; i < moveCommandCount; ++i) {
        memory.Write(ProcessorInstruction::PushStackValue, Direction::Right);
        memory.Write(ProcessorInstruction::Call, move);
    }
    for (uint8_t i = 0; i < moveCommandCount; ++i) {
        memory.Write(ProcessorInstruction::PushStackValue, Direction::Left);
        memory.Write(ProcessorInstruction::Call, move);
    }
    memory.Write(ProcessorInstruction::Jump, std::byte { 0 });
    return brain;
}
