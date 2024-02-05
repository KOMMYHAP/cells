#pragma once

#include "spawn_properties.h"

class PositionSystem;
class BrainSystem;
class IdSystem;
class TypeSystem;
class HealthSystem;
class AgeSystem;
class IdSystem;

class Spawner {
public:
    Spawner(PositionSystem& positionSystem, TypeSystem& typeSystem, BrainSystem& brainSystem, HealthSystem& healthSystem, AgeSystem& ageSystem, IdSystem& idSystem);

    CellId TrySpawn(const SpawnProperties& properties);

private:
    PositionSystem& _positionSystem;
    TypeSystem& _typeSystem;
    BrainSystem& _brainSystem;
    HealthSystem& _healthSystem;
    AgeSystem& _ageSystem;
    IdSystem& _idSystem;
};
