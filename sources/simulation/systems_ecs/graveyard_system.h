#pragma once

#include "simulation/simulation_ecs_system.h"

class GraveyardSystem final : public SimulationSystem {
public:
    explicit GraveyardSystem(EcsWorld& world);

    void DoSystemUpdate() override;

private:
    gsl::not_null<EcsWorld*> _world;
};
