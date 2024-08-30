﻿#pragma once

#include "simulation/simulation_ecs_system.h"
#include "simulation/simulation_statistics_provider.h"

class DeathFromAgeStatisticsSystem final : public SimulationSystem {
public:
    explicit DeathFromAgeStatisticsSystem(EcsWorld& world, SimulationStatisticsProvider& stats);

    void DoSystemUpdate() override;

private:
    gsl::not_null<EcsWorld*> _world;
    gsl::not_null<SimulationStatisticsProvider*> _stats;
};