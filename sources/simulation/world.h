#pragma once

#include "systems_ecs/simulation_ecs_system.h"

#include "random/random.h"
#include "sample_counter.h"

#include "simulation/cell_locator.h"
#include "simulation/simulation_system.h"
#include "simulation/simulation_virtual_machine.h"

#include "cell_factories/patrol_cell_factory.h"
#include "tick_calculator.h"

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
    common::SampleCounter<float, 20> _tickSampler;
    SimulationTickCalculator _tickCalculator;
    std::vector<std::unique_ptr<SimulationSystem>> _simulationSystems;
    sf::Vector2u _worldSize;
    CellLocator _cellsLocator;
    SimulationVirtualMachine _simulationVm;
    Random::Engine _randomEngine;
    PatrolCellFactory _cellFactory;
};