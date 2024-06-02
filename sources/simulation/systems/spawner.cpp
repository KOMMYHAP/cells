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
      , _idSystem(idSystem) {}

CellId Spawner::TrySpawn(const SpawnProperties& properties)
{
    const CellId id = _idSystem.Create();
    if (id == CellId::Invalid) {
        return CellId::Invalid;
    }
    Expects(properties.position != InvalidCellPosition);
    Expects(properties.type != CellType::Dummy);
    Expects(properties.health != CellHealth::Zero);

    _brainSystem.Access(id) = properties.brain;
    _positionSystem.Set(id, properties.position);
    _typeSystem.Set(id, properties.type);
    _ageSystem.Set(id, properties.age);
    _healthSystem.Set(id, properties.health);
    return id;
}