#pragma once

#include "components/cell_age.h"
#include "simulation/simulation_ecs_system.h"

class AgeSystem final : public SimulationEcsSystem<AgeSystem, CellAge> {
public:
    explicit AgeSystem(EcsWorld& ecsWorld);

    void DoProcessComponents(EcsEntity id, CellAge& age);
};
