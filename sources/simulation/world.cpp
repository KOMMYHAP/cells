#include "world.h"

#include "storage/stack_storage.h"

std::error_code World::InitializeSystem(common::StackStorage& storage)
{
    _parameters = &storage.Modify<WorldParameters>();
    _simulation = std::make_unique<Simulation>(*_parameters->simulationScript);
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
