#include "simulation_registrable_system.h"

#include "world.h"

SimulationRegistrableSystem::SimulationRegistrableSystem() = default;
SimulationRegistrableSystem::~SimulationRegistrableSystem() = default;

std::error_code SimulationRegistrableSystem::InitializeSystem(ApplicationStorage& storage)
{
    const SimulationConfig& config = storage.Get<SimulationConfig>();
    storage.Store<World>(config);
    return {};
}

void SimulationRegistrableSystem::TerminateSystem()
{
}