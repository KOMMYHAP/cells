#pragma once

#include "components/cell_age.h"
#include "components/cell_id.h"
#include "simulation/simulation_ecs_system.h"

class System;
class HealthSystem;

class AgeSystem final : public SimulationEcsSystem<AgeSystem, CellAge> {
public:
    explicit AgeSystem(EcsWorld& ecsWorld);

    void DoProcessComponents(EcsEntity id, CellAge& age);
};
