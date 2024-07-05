#include "brain_simulation_system.h"

BrainSimulationSystem::BrainSimulationSystem(EcsWorld& ecsWorld, SimulationVirtualMachine& vm)
    : SimulationEcsSystem(ecsWorld)
    , _vm(&vm)
{
}

void BrainSimulationSystem::DoProcessComponents(const CellId id, CellBrain& brain)
{
    _vm->Run(id, brain);
}