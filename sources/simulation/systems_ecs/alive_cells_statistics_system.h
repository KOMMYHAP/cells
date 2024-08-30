#pragma once

#include "simulation/simulation_ecs_system.h"
#include "simulation/simulation_statistics_provider.h"

class AliveCellsStatisticsSystem final : public SimulationSystem {
public:
    explicit AliveCellsStatisticsSystem(EcsWorld& ecsWorld, SimulationStatisticsProvider& stats);

    void DoSystemUpdate() override;

private:
    gsl::not_null<EcsWorld*> _world;
    gsl::not_null<SimulationStatisticsProvider*> _stats;
};
