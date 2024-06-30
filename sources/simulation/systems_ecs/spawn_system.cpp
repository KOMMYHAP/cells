#include "spawn_system.h"

#include "components/cell_brain.h"
#include "components/cell_type.h"
#include "components/move_direction.h"
#include "components/procedure_type.h"

#include "components/spawn_place.h"
#include "processor/processor_control_block.h"
#include "processor/processor_instruction.h"
#include "processor/processor_memory.h"

SpawnSystem::SpawnSystem(EcsWorld& ecsWorld, Random::Accessor random, const SimulationVirtualMachine& vm)
    : SimulationEcsSystem(ecsWorld)
    , _simulationVm(&vm)
    , _random(std::move(random))
{
}

void SpawnSystem::DoProcessComponents(const CellId id, CellType type)
{
    switch (type) {

    case CellType::Unit:
        SpawnUnit(id);
        break;
    case CellType::Food:
        SpawnFood(id);
        break;
    case CellType::Wall:
    case CellType::Dummy:
        ASSERT("not implemented");
        break;
    }

    AccessEcsWorld().remove<SpawnPlace>(id);
}

void SpawnSystem::SpawnUnit(CellId id)
{
    EcsWorld& ecsWorld = AccessEcsWorld();
    const bool hasPrototypeComponents = ecsWorld.any_of<CellBrain, MoveDirection>(id);
    ASSERT(!hasPrototypeComponents);

    auto& brain = ecsWorld.emplace<CellBrain>(id);
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

    const ProcedureId move = _simulationVm->GetProcedureId(ProcedureType::Move);

    constexpr int moveCommandCount { 10 };
    for (int i = 0; i < moveCommandCount; ++i) {
        memory.Write(ProcessorInstruction::PushStackValue, MoveDirection::Right);
        memory.Write(ProcessorInstruction::Call, move);
    }
    for (uint8_t i = 0; i < moveCommandCount; ++i) {
        memory.Write(ProcessorInstruction::PushStackValue, MoveDirection::Left);
        memory.Write(ProcessorInstruction::Call, move);
    }
    memory.Write(ProcessorInstruction::Jump, std::byte { 0 });

    constexpr MoveDirection possibleDirections[] = { MoveDirection::Down, MoveDirection::Up, MoveDirection::Left, MoveDirection::Right };
    const MoveDirection selectedDirection = _random.Select(std::span(possibleDirections));
    ecsWorld.emplace<MoveDirection>(id, selectedDirection);
}

void SpawnSystem::SpawnFood(CellId /*id*/)
{
}
