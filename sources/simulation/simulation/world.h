#pragma once

#include "systems_ecs/simulation_ecs_system.h"

#include <entt/entity/registry.hpp>

#include "random/random.h"
#include "simulation/simulation_system.h"
#include "simulation/tick_calculator.h"
#include "systems_ecs/cell_locator.h"
#include "systems_ecs/simulation_virtual_machine.h"

class World {
public:
    World();

    void Update(sf::Time elapsedTime);

    const EcsWorld& GetEcsWorld() const { return _ecsWorld; }
    EcsWorld& ModifyEcsWorld() { return _ecsWorld; }

    sf::Vector2u GetWorldSize() const { return _worldSize; }

private:
    EcsWorld _ecsWorld;
    SimulationTickCalculator _tickCalculator;
    std::vector<std::unique_ptr<SimulationSystem>> _simulationSystems;
    sf::Vector2u _worldSize;
    CellLocator _cellsOnCurrentTick;
    CellLocator _cellsOnNextTick;
    SimulationVirtualMachine _simulationVm;
    Random::Engine _randomEngine;
};