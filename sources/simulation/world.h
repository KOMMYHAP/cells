#pragma once

#include "sample_counter.h"
#include "systems_ecs/simulation_ecs_system.h"

#include <entt/entity/registry.hpp>

#include "random/random.h"
#include "simulation/simulation_system.h"
#include "simulation/tick_calculator.h"
#include "simulation/cell_locator.h"
#include "simulation/simulation_virtual_machine.h"

class World {
public:
    World();

    void Update(sf::Time elapsedTime);

    const EcsWorld& GetEcsWorld() const { return _ecsWorld; }
    EcsWorld& ModifyEcsWorld() { return _ecsWorld; }

    sf::Vector2u GetWorldSize() const { return _worldSize; }

private:
    void Warmup();
    sf::Time GetTickTime() const;
    void Tick();
    
    EcsWorld _ecsWorld;
    common::SampleCounter<float, 100> _tickSampler;
    SimulationTickCalculator _tickCalculator;
    std::vector<std::unique_ptr<SimulationSystem>> _simulationSystems;
    sf::Vector2u _worldSize;
    CellLocator _cellsLocator;
    SimulationVirtualMachine _simulationVm;
    Random::Engine _randomEngine;
};