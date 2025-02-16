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

#include "simulation/simulation_storage.h"

class World {
public:
    World();

    SimulationStorage& ModifySimulation() { return _simulationStorage; }
    void AddSimulationSystem(std::unique_ptr<SimulationSystem> system);

    // explicit World(const SimulationConfig& config);

    void Update(Common::Time elapsedTime);

    // const EcsWorld& GetEcsWorld() const { return _ecsWorld; }
    // EcsWorld& ModifyEcsWorld() { return _ecsWorld; }

    // const SimulationStatisticsProvider& GetSimulationStatistics() const { return _statistics; }

private:
    void Warmup();
    Common::Time GetTickTime() const;
    void Tick();

    // template <class T, class... Args>
    //     requires std::is_base_of_v<ProcedureBase, T> && std::is_base_of_v<SimulationSystem, T> && std::is_constructible_v<T, Args...>
    // T& RegisterProcedureSystem(ProcedureType type, uint8_t inputCount, uint8_t outputCount, std::string name, Args&&... args);
    //
    // template <class T, class... Args>
    //     requires std::is_base_of_v<SimulationSystem, T> && std::is_constructible_v<T, Args...>
    // T& RegisterSystem(Args&&... args);

    common::SampleCounter<Common::Time, 20> _tickSampler;
    SimulationTickCalculator _tickCalculator;
    SimulationStorage _simulationStorage;
    std::vector<std::unique_ptr<SimulationSystem>> _simulationSystems;
    // Random::Engine _randomEngine;
    // EcsWorld _ecsWorld;
    // CellLocator _cellsLocator;
    // Spawner _spawner;
    // SimulationVirtualMachine _simulationVm;
    // RandomCellFactory _randomCellFactory;
    // SimulationStatisticsProvider _statistics;
};
