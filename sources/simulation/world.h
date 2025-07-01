#pragma once

#include "simulation/simulation_ecs_system.h"

#include "simulation/simulation_system.h"
#include "tick_calculator.h"

#include "simulation/common_adapter.h"

class WorldStatistics;

class World {
public:
    World(WorldStatistics& stats);

    SimulationStorage& ModifySimulation() { return _simulationStorage; }
    void AddSimulationSystem(std::unique_ptr<SimulationSystem> system);

    void Update(Common::Time elapsedTime);

private:
    Common::Time GetTickTime() const;
    void Tick();

    gsl::not_null<WorldStatistics*> _worldStatistics;
    SimulationTickCalculator _tickCalculator;
    SimulationStorage _simulationStorage;
    std::vector<std::unique_ptr<SimulationSystem>> _simulationSystems;
};
