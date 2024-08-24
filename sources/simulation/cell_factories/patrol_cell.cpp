#include "patrol_cell.h"

#include "components/direction.h"
#include "procedures/procedure_context.h"
#include "processor/processor_control_block.h"
#include "processor/processor_instruction.h"
#include "processor/processor_state.h"

PatrolCellFactory::PatrolCellFactory(const SimulationVirtualMachine& vm)
    : _vm(&vm)
{
}

ICellFactory::Result PatrolCellFactory::Make(const Parent& parent)
{
    Result child;
    child.success = true;

    CellBrain& brain = child.brain;
    ProcessorMemory memory { brain.data };
    ProcessorControlBlock controlBlock {
        static_cast<std::underlying_type_t<ProcessorState>>(ProcessorState::Good),
        static_cast<std::underlying_type_t<ProcedureId>>(ProcedureId::Invalid),
        0,
        0,
        {},
        0,
        {}
    };
    memory.Write(controlBlock);

    ASSERT(ProcessorRegistryCount >= 1);

    const ProcedureId move = _vm->GetProcedureId(ProcedureType::Move);
    const ProcedureId look = _vm->GetProcedureId(ProcedureType::Look);

    constexpr int moveCommandCount { 10 };
    for (int i = 0; i < moveCommandCount; ++i) {
        memory.Write(ProcessorInstruction::PushStackValue, Direction::Right);
        memory.Write(ProcessorInstruction::Call, look);
        memory.Write(ProcessorInstruction::PopStackRegistry, std::byte { 0 });
        memory.Write(ProcessorInstruction::PushStackValue, Direction::Right);
        memory.Write(ProcessorInstruction::Call, move);
    }
    for (uint8_t i = 0; i < moveCommandCount; ++i) {
        memory.Write(ProcessorInstruction::PushStackValue, Direction::Left);
        memory.Write(ProcessorInstruction::Call, look);
        memory.Write(ProcessorInstruction::PopStackRegistry, std::byte { 0 });
        memory.Write(ProcessorInstruction::PushStackValue, Direction::Left);
        memory.Write(ProcessorInstruction::Call, move);
    }
    memory.Write(ProcessorInstruction::Jump, std::byte { 0 });

    return child;
}
