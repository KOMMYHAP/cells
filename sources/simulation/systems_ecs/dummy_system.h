#pragma once

#include "simulation_ecs_system.h"

class DummySystem final : public SimulationEcsSystem<DummySystem, DummyComponent> {
public:
    explicit DummySystem(EcsWorld& ecsWorld);

    void DoProcessComponents(CellId id, DummyComponent dummy);
private:
};
