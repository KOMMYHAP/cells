#include "world.h"

#include "entt/entt.hpp"

#include "storage/stack_storage.h"

std::error_code World::InitializeSystem(common::StackStorage& storage)
{
    _ecsWorld = std::make_unique<EcsWorld>();
    _simulation = std::make_unique<Simulation>();
    storage.Store<World*>(this);
    return {};
}

void World::TerminateSystem()
{
    _simulation.reset();
    _ecsWorld.reset();
}

void World::Update(sf::Time elapsedTime)
{
    _simulation->Run(elapsedTime);
}
