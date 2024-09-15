#pragma once

#include "simulation/simulation_ecs_system.h"

#include "random/random.h"
#include "sample_counter.h"

#include "cell_factories/random_cell_factory.h"
#include "simulation/cell_locator.h"
#include "simulation/simulation_statistics_provider.h"
#include "simulation/simulation_system.h"
#include "simulation/simulation_virtual_machine.h"
#include "simulation/spawner.h"
#include "simulation_config.h"
#include "tick_calculator.h"

class World {
public:
    explicit World(const SimulationConfig& config);

    void Update(sf::Time elapsedTime);

    const EcsWorld& GetEcsWorld() const { return _ecsWorld; }
    EcsWorld& ModifyEcsWorld() { return _ecsWorld; }

    const SimulationStatisticsProvider& GetSimulationStatistics() const { return _statistics; }

private:
    void Warmup();
    sf::Time GetTickTime() const;
    void Tick();

    template <class T, class... Args>
        requires std::is_base_of_v<ProcedureBase, T> && std::is_base_of_v<SimulationSystem, T> && std::is_constructible_v<T, Args...>
    T& RegisterProcedureSystem(ProcedureType type, uint8_t inputCount, uint8_t outputCount, std::string name, Args&&... args);

    template <class T, class... Args>
        requires std::is_base_of_v<SimulationSystem, T> && std::is_constructible_v<T, Args...>
    T& RegisterSystem(Args&&... args);

    EcsWorld _ecsWorld;
    common::SampleCounter<float, 20> _tickSampler;
    SimulationTickCalculator _tickCalculator;
    CellLocator _cellsLocator;
    Spawner _spawner;
    std::vector<std::unique_ptr<SimulationSystem>> _simulationSystems;
    SimulationVirtualMachine _simulationVm;
    Random::Engine _randomEngine;
    RandomCellFactory _randomCellFactory;
    SimulationStatisticsProvider _statistics;
};
