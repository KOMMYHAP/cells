#include "spawn_system.h"

#include "cell_factories/cell_factory_interface.h"
#include "random.h"
#include "spawn_properties.h"
#include "systems/id_system.h"
#include "systems/position_system.h"
#include "systems/spawner.h"

SpawnSystem::SpawnSystem(SpawnSystem::Config&& config)
    : _positionSystem(config.positionSystem)
    , _idSystem(config.idSystem)
    , _targetPopulationSize(config.populationSize)
    , _spawner(config.spawner)
{
}

void SpawnSystem::SpawnN(uint32_t cellsCount)
{
    if (cellsCount == 0) {
        return;
    }

    std::vector<CellPosition> positions = _positionSystem.CollectFreePositions();
    std::shuffle(positions.begin(), positions.end(), common::GetRandomEngine());
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
    if (!_cellFactory) {
        assert(false);
        return {};
    }

    ICellFactory::Result result = _cellFactory->Make();
    if (result.status == ICellFactory::Status::FailedToCreate) {
        assert(false);
        return {};
    }
    if (result.status == ICellFactory::Status::TryLater) {
        return {};
    }

    return result.brain;
}
