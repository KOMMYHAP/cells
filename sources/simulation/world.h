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

    enum class Phase {
        Stopped,
        Running,
        Paused
    };
    void AddSimulationSystem(Phase phase, std::unique_ptr<SimulationSystem> system);

    void Update(Common::Time elapsedTime);

    void SetPhase(Phase phase);
    Phase GetPhase() const { return _activePhase; }

private:
    using Systems = std::vector<std::unique_ptr<SimulationSystem>>;
    Common::Time GetTickTime() const;
    void Tick();

    gsl::not_null<WorldStatistics*> _worldStatistics;
    SimulationTickCalculator _tickCalculator;
    SimulationStorage _simulationStorage;
    Phase _activePhase{Phase::Stopped};
    std::map<Phase, Systems> _simulationSystems;
};
