#include "spawn_system.h"

#include "cell_factories/cell_factory_interface.h"
#include "components/cell_age.h"
#include "components/cell_brain.h"
#include "components/cell_health.h"
#include "components/cell_type.h"
#include "components/move_direction.h"
#include "components/procedure_type.h"
#include "entt/entt.hpp"

#include "components/spawn_place.h"
#include "processor/processor_control_block.h"
#include "processor/processor_memory.h"

SpawnSystem::SpawnSystem(EcsWorld& ecsWorld, ICellFactory& cellFactory)
    : EcsSimulationSystem(ecsWorld)
    , _cellFactory(&cellFactory)
{
}

void SpawnSystem::DoProcessComponents(CellId id, CellPosition position)
{
    EcsWorld& ecsWorld = AccessEcsWorld();

    const bool hasPrototypeComponents = ecsWorld.any_of<CellBrain, CellType, CellHealth, CellAge>(id);
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

    constexpr CellHealth initialHealth { 100 };
    ecsWorld.emplace<CellBrain>(id, brain);
    ecsWorld.emplace<CellType>(id, CellType::Unit);
    ecsWorld.emplace<CellHealth>(id, initialHealth);
    ecsWorld.emplace<CellAge>(id, CellAge::Zero);
    ecsWorld.emplace<CellPosition>(id, position);

    constexpr MoveDirection possibleDirections[] = { MoveDirection::Down, MoveDirection::Up, MoveDirection::Left, MoveDirection::Right };
    const MoveDirection selectedDirection = random::Select(std::span(possibleDirections));
    ecsWorld.emplace<MoveDirection>(id, selectedDirection);

    ecsWorld.remove<SpawnPlace>(id);
}
