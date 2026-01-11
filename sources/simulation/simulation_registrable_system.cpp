#include "simulation_registrable_system.h"

#include "world.h"
#include "world_statistics.h"

SimulationRegistrableSystem::SimulationRegistrableSystem() = default;
SimulationRegistrableSystem::~SimulationRegistrableSystem() = default;

std::error_code SimulationRegistrableSystem::InitializeSystem(ApplicationStorage& storage)
{
    storage.Store<World>();
    return {};
}

void SimulationRegistrableSystem::TerminateSystem()
{
}