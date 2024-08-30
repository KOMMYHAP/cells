#pragma once

#include "simulation/simulation_ecs_system.h"

class SpawnPlacesStatisticsSystem final : public SimulationSystem {
public:
    explicit SpawnPlacesStatisticsSystem(EcsWorld& ecsWorld);

    void DoSystemUpdate() override;

    size_t GetSpawnPlacesCount() const { return _spawnPlaces; }

private:
    size_t _spawnPlaces { 0 };
    gsl::not_null<EcsWorld*> _world;
};
