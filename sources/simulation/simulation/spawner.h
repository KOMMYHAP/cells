#pragma once
#include "cell_locator.h"

class Spawner {
public:
    Spawner(EcsWorld& world, const CellLocator& locator);

    EcsEntity ScheduleSpawn(CellPosition position);

private:
    gsl::not_null<EcsWorld*> _world;
    gsl::not_null<const CellLocator*> _locator;
};