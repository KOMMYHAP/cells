#include "spawn_system.h"

#include "cell_factories/cell_factory_interface.h"
#include "spawn_properties.h"
#include "storage/storage.h"
#include "systems/id_system.h"
#include "systems/position_system.h"
#include "systems/spawner.h"

SpawnSystem::SpawnSystem(PositionSystem& positionSystem, IdSystem& idSystem, Spawner& spawner)
    : _positionSystem(positionSystem)
    , _idSystem(idSystem)
    , _spawner(spawner)
{
}

void SpawnSystem::SpawnN(uint32_t cellsCount)
{
    if (cellsCount == 0) {
        return;
    }

    std::vector<CellPosition> positions = _positionSystem.CollectFreePositions();
    // common::Shuffle(positions);
    uint32_t spawnedCount { 0 };

    constexpr CellHealth initialHealth { 100 };

    for (const CellPosition& position : positions) {
        const auto mbBrain = TryMakeCellBrain();
        if (!mbBrain.has_value()) {
            break;
        }

        SpawnProperties properties;
        properties.position = position;
        properties.health = initialHealth;
        properties.type = CellType::Unit;
        properties.age = CellAge::Zero;
        properties.brain = *mbBrain;

        const CellId id = _spawner.TrySpawn(properties);
        if (id == CellId::Invalid) {
            break;
        }

        spawnedCount += 1;
        if (spawnedCount == cellsCount) {
            break;
        }
    }
}

void SpawnSystem::SetCellFactory(ICellFactory& factory)
{
    _cellFactory = &factory;
}

void SpawnSystem::TryToSpawn()
{
    if (_idSystem.GetCellsCount() >= _targetPopulationSize) {
        return;
    }

    const uint32_t cellsCount = _targetPopulationSize - _idSystem.GetCellsCount();
    SpawnN(cellsCount);
}

std::optional<CellBrain> SpawnSystem::TryMakeCellBrain()
{
    ASSERT(_cellFactory);

    ICellFactory::Result result = _cellFactory->Make();
    ASSERT((result.status != ICellFactory::Status::FailedToCreate));

    if (result.status == ICellFactory::Status::TryLater) {
        return {};
    }
    return result.brain;
}

void SpawnSystem::SetSpawnLimit(uint32_t populationSize)
{
    _targetPopulationSize = populationSize;
}