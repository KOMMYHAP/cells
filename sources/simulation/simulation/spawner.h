#pragma once
#include "cell_locator.h"
#include "components/cell_brain.h"
#include "components/direction.h"

template <class Func>
concept CellFactory = std::is_invocable_r_v<bool, Func, CellBrain&>;

class Spawner {
public:
    Spawner(EcsWorld& world, CellLocator& locator);

    template <CellFactory Factory>
    bool TrySpawn(CellPosition targetPosition, Factory factory);

    template <CellFactory Factory>
    bool TrySpawn(CellPosition parentPosition, Direction direction, Factory factory);

private:
    bool CanSpawnAtPosition(CellPosition position) const;
    void Spawn(const CellBrain& brain, CellPosition position);

    gsl::not_null<EcsWorld*> _world;
    gsl::not_null<CellLocator*> _locator;
};

#include "spawner.hpp"