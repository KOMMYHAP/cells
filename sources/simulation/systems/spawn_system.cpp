#include "spawn_system.h"

#include "components/cell_brain.h"
#include "components/cell_id.h"
#include "components/cell_type.h"
#include "random.h"
#include "systems/cell_factory.h"
#include "systems/id_system.h"
#include "systems/position_system.h"
#include "systems/type_system.h"

SpawnSystem::SpawnSystem(SpawnSystem::Config&& config)
    : _factory(config.factory)
    , _positionSystem(config.positionSystem)
    , _targetPopulationSize(config.targetPopulationSize)
    , _idSystem(config.idSystem)
    , _typeSystem(config.typeSystem)
    , _spawnPolicy(config.policy)
{
}

void SpawnSystem::Tick()
{
    if (_idSystem.GetCellsCount() >= _targetPopulationSize) {
        return;
    }

    SpawnN(_targetPopulationSize - _idSystem.GetCellsCount());
}

void SpawnSystem::SpawnN(uint32_t cellsCount)
{
    std::vector<CellPosition> positions = _positionSystem.CollectFreePositions();
    std::shuffle(positions.begin(), positions.end(), common::GetRandomEngine());
    if (positions.size() > cellsCount) {
        positions.resize(cellsCount);
    }

    for (const CellPosition& position : positions) {
        const CellId id = _idSystem.Create();
        _positionSystem.Set(id, position);
        _typeSystem.Set(id, CellType::Unit);

        const bool created = SpawnUnit(id);
        assert(created);
    }
}

bool SpawnSystem::SpawnUnit(CellId id)
{
    if (_spawnPolicy == Policy::RandomUnit) {
        return _factory.MakeRandomUnit(id, CellBrainLimit);
    }
    if (_spawnPolicy == Policy::PatrolUnit) {
        const uint16_t moveCommandsCount = std::min<uint16_t>(_positionSystem.GetWidth(), 3);
        return _factory.MakePatrolUnit(id, moveCommandsCount);
    }
    return false;
}
