#include "spawn_system.h"

#include "components/cell_brain.h"
#include "components/cell_type.h"
#include "components/move_direction.h"
#include "components/procedure_type.h"

#include "components/spawn_data.h"
#include "processor/processor_control_block.h"
#include "processor/processor_instruction.h"
#include "processor/processor_memory.h"

SpawnSystem::SpawnSystem(EcsWorld& ecsWorld, Random::Accessor random, const SimulationVirtualMachine& vm)
    : SimulationEcsSystem(ecsWorld)
    , _simulationVm(&vm)
    , _random(std::move(random))
{
}

void SpawnSystem::DoProcessComponents(const CellId id, JustSpawned data)
{
    EcsWorld& ecsWorld = AccessEcsWorld();

    AccessEcsWorld().remove<JustSpawned>(id);
}

void SpawnSystem::SpawnUnit(CellId id)
{
}

void SpawnSystem::SpawnFood(CellId /*id*/)
{
}
