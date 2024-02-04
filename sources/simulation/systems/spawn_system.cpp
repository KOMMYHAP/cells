#include "spawn_system.h"

#include "breakpoint.h"
#include "cell_factories/cell_factory_interface.h"
#include "components/cell_brain.h"
#include "components/cell_id.h"
#include "components/cell_type.h"
#include "random.h"
#include "systems/brain_system.h"
#include "systems/id_system.h"
#include "systems/position_system.h"
#include "systems/type_system.h"

SpawnSystem::SpawnSystem(SpawnSystem::Config&& config)
    : _positionSystem(config.positionSystem)
    , _idSystem(config.idSystem)
    , _typeSystem(config.typeSystem)
    , _brainSystem(config.brainSystem)
    , _targetPopulationSize(config.populationSize)
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

    for (const CellPosition& position : positions) {
        const CellId id = _idSystem.Create();
        const bool wasSpawned = SpawnUnit(id);
        if (!wasSpawned) {
            _idSystem.Remove(id);
            continue;
        }

        _positionSystem.Set(id, position);
        _typeSystem.Set(id, CellType::Unit);
        spawnedCount += 1;
        if (spawnedCount == cellsCount) {
            break;
        }
    }
}

bool SpawnSystem::SpawnUnit(CellId id)
{
    if (!_cellFactory) {
        assert(false);
        return false;
    }

    ICellFactory::Result result = _cellFactory->Make();
    if (result.status == ICellFactory::Status::FailedToCreate) {
        assert(false);
        return false;
    }
    if (result.status == ICellFactory::Status::TryLater) {
        return false;
    }

    _brainSystem.Access(id) = result.brain;
    return true;
}

void SpawnSystem::DebugDumpAliveCells()
{
    // debug only: at least one cell is alive!!!!!
    std::cout << "Alive cells:\n";
    _idSystem.Iterate([this](const CellId id) {
        _brainSystem.DumpCellBrain(std::cout, id);
        std::cout << "\n";
    });
    std::cout << std::endl;
    common::Breakpoint();
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
