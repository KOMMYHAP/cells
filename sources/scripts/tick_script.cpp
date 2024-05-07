#include "tick_script.h"

#include "storage.h"

#include "cell_factories/patrol_cell_factory.h"
#include "cell_factories/random_cell_factory.h"
#include "procedures/consume_procedure.h"
#include "procedures/look_procedure.h"
#include "procedures/move_procedure.h"
#include "procedures/reproduction_procedure.h"
#include "systems/age_system.h"
#include "systems/brain_system.h"
#include "systems/graveyard_system.h"
#include "systems/health_system.h"
#include "systems/id_system.h"
#include "systems/position_system.h"
#include "systems/selection_system.h"
#include "systems/simulation_virtual_machine.h"
#include "systems/spawn_system.h"
#include "systems/spawner.h"
#include "systems/type_system.h"

struct Statistics {
    uint32_t cellsCount { 0 };
    uint32_t generation { 0 };
    uint64_t tick { 0 };
};
static Statistics __statistics;

struct TickScript::Parameters {
    enum class SpawnPolicy {
        Random,
        Patrol
    };

    CellAge limitCellAge { 100 };
    uint16_t bestCellSelectionSize { 100 };
    uint16_t selectionEpochTicks { 1000 };
    SpawnPolicy spawnPolicy { SpawnPolicy::Random };

    std::unique_ptr<PatrolCellFactory> patrolCellFactory;
    std::unique_ptr<RandomCellFactory> randomCellFactory;
    ICellFactory* cellFactory { nullptr };
};

TickScript::TickScript(const common::Storage& systems)
    : _systems(systems)
    , _parameters(std::make_unique<Parameters>())
{
}

void TickScript::Perform()
{
    auto& idSystem = _systems.Modify<IdSystem>();
    auto& ageSystem = _systems.Modify<AgeSystem>();
    auto& simulationVm = _systems.Modify<SimulationVirtualMachine>();
    auto& graveyardSystem = _systems.Modify<GraveyardSystem>();
    auto& spawnSystem = _systems.Modify<SpawnSystem>();
    auto& selectionSystem = _systems.Modify<SelectionSystem>();

    // Process brain of each cell.
    idSystem.Iterate([&](const CellId id) {
        simulationVm.Run(id);
    });

    // Kill too old cells
    idSystem.Iterate([&](const CellId id) {
        ageSystem.Increment(id, _parameters->limitCellAge);
    });

    // Cleanup world systems from dead cells.
    graveyardSystem.Cleanup();

    // Spawn more if needed
    auto respawn = [&]() {
        idSystem.Iterate([&](const CellId id) {
            graveyardSystem.Bury(id);
        });
        graveyardSystem.Cleanup();

        spawnSystem.TryToSpawn();
    };
    const uint32_t aliveCellsCount = idSystem.GetCellsCount();
    if (aliveCellsCount == 0) {
        selectionSystem.Restart();
        spawnSystem.SetCellFactory(*_parameters->cellFactory);
        __statistics.generation = 0;
        respawn();
    } else {
        SelectionSystem::Result selectionResult = selectionSystem.TickGeneration();
        if (selectionResult.shouldRespawn) {
            spawnSystem.SetCellFactory(*selectionResult.cellFactory);
            __statistics.generation = selectionResult.generation;
            respawn();
        }
    }

    __statistics.cellsCount = idSystem.GetCellsCount();
    __statistics.tick += 1;
}
