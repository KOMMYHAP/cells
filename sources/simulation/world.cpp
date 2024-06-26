#include "world.h"

#include "simulation_parameters.h"
#include "storage/stack_storage.h"

std::error_code World::InitializeSystem(common::StackStorage& storage)
{
    _parameters = &storage.Modify<WorldParameters>();
    const auto& simulationParameters = storage.Get<SimulationParameters>();
    _simulation = std::make_unique<Simulation>(*_parameters->simulationScript);
    _simulation->SetAutoMode(simulationParameters.targetSimulationTime);
    storage.Store<World*>(this);
    return {};
}

void World::TerminateSystem()
{
    _simulation.reset();
    _parameters = nullptr;
}

void World::Update(sf::Time elapsedTime)
{
    _simulation->Run(elapsedTime);
}
