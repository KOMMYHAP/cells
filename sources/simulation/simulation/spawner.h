#pragma once
#include "cell_locator.h"

class Spawner {
public:
    explicit Spawner(EcsWorld& world);

    CellId ScheduleSpawn(CellPosition position);

private:
    gsl::not_null<EcsWorld*> _world;
};