#include "spawner.h"

#include "spawn_properties.h"
#include "systems/age_system.h"
#include "systems/brain_system.h"
#include "systems/health_system.h"
#include "systems/id_system.h"
#include "systems/position_system.h"
#include "systems/type_system.h"

Spawner::Spawner(PositionSystem& positionSystem, TypeSystem& typeSystem, BrainSystem& brainSystem, HealthSystem& healthSystem, AgeSystem& ageSystem, IdSystem& idSystem)
    : _positionSystem(positionSystem)
    , _typeSystem(typeSystem)
    , _brainSystem(brainSystem)
    , _healthSystem(healthSystem)
    , _ageSystem(ageSystem)
    , _idSystem(idSystem)
{
}

CellId Spawner::TrySpawn(const SpawnProperties& properties)
{
    const CellId id = _idSystem.Create();
    if (id == CellId::Invalid) {
        return CellId::Invalid;
    }

    _brainSystem.Access(id) = properties.brain;

    assert(properties.position != InvalidCellPosition);
    _positionSystem.Set(id, properties.position);

    assert(properties.type != CellType::Dummy);
    _typeSystem.Set(id, properties.type);

    _ageSystem.Set(id, properties.age);

    assert(properties.health != CellHealth::Zero);
    _healthSystem.Set(id, properties.health);
    return id;
}