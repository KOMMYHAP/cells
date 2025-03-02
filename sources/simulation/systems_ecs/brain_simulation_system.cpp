#include "generated/auto_brain_simulation_system.h"
#include "simulation/simulation_virtual_machine.h"

void BrainSimulationSystem::DoProcessComponents(const EcsEntity id, CellBrain& brain)
{
    _simulationVirtualMachine->Run(id, brain);
}