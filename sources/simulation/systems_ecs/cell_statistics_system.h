#pragma once

#include "simulation/simulation_ecs_system.h"

class AliveCellsStatisticsSystem final : public SimulationSystem {
public:
    explicit AliveCellsStatisticsSystem(EcsWorld& ecsWorld);

    void DoSystemUpdate() override;

    size_t GetAliveCellsCount() const { return _aliveCells; }

private:
    size_t _aliveCells { 0 };
    gsl::not_null<EcsWorld*> _world;
};
