#include "reproduction_system.h"

#include "components/procedure_result.h"
#include "components/procedure_type.h"
#include "processor/processor_control_block.h"
#include "processor/processor_instruction.h"
#include "processor/processor_memory.h"

ReproductionSystem::ReproductionSystem(EcsWorld& ecsWorld, const SimulationVirtualMachine& vm, const CellLocator& locator, Random::Accessor random)
    : SimulationEcsSystem(ecsWorld)
    , _locator(&locator)
    , _vm(&vm)
    , _random(random)
{
}

void ReproductionSystem::DoProcessComponents(CellId id, const CellPosition position, const ReproductionDirection direction, ProcedureResult& result)
{
    const CellPosition childPosition = _locator->TryApplyDirection(position, direction.value);
    if (childPosition == InvalidCellPosition) {
        result.context.MarkProcedureAsInvalid();
        return;
    }

    if (const CellId targetId = _locator->Find(childPosition); targetId != CellId::Invalid) {
        result.context.MarkProcedureAsInvalid();
        return;
    }

    EcsWorld& world = AccessEcsWorld();
    const CellId child = world.create();
    world.emplace<CellBrain>(child, MakeTempBrain());
    world.emplace<CellPosition>(child, childPosition);
}

CellBrain ReproductionSystem::MakeTempBrain()
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

    const ProcedureId move = _vm->GetProcedureId(ProcedureType::Move);

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