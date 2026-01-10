#pragma once

#include "simulation/simulation_ecs_system.h"

#include "conditions/condition.h"
#include "conditions/condition_system.h"
#include "simulation/simulation_system.h"

#include "simulation/common_adapter.h"
#include "simulation/simulation_player.h"

class WorldStatistics;

class World {
public:
    World(WorldStatistics& stats);

    SimulationStorage& ModifySimulation() { return _simulationStorage; }

    void AddSimulationSystem(Common::Condition condition, std::unique_ptr<SimulationSystem> system);

    void Update(Common::Time elapsedTime);

    const SimulationPlayer& GetSimulationPlayer() const { return _player; }
    SimulationPlayer& ModifySimulationPlayer() { return _player; }

    const Common::ConditionSystem& GetConditionSystem() const { return _conditions; }
    Common::ConditionSystem& ModifyConditionSystem() { return _conditions; }

private:
    struct SystemData {
        Common::Condition condition { Common::Condition::Invalid };
        std::unique_ptr<SimulationSystem> system;
    };

    void SimulateStep();

    gsl::not_null<WorldStatistics*> _worldStatistics;
    SimulationPlayer _player;
    SimulationStorage _simulationStorage;
    Common::ConditionSystem _conditions;
    std::vector<SystemData> _systems;
};
