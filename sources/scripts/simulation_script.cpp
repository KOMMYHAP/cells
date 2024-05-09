#include "simulation_script.h"

#include "storage.h"

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

SimulationScript::SimulationScript(const common::Storage& systems)
    : _systems(systems)
{
}

std::expected<void, std::error_code> SimulationScript::Perform()
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
        ageSystem.Increment(id, _parameters.limitCellAge);
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
        spawnSystem.SetCellFactory(*_cellFactory);
        _stats.generation = 0;
        respawn();
    } else {
        SelectionSystem::Result selectionResult = selectionSystem.TickGeneration();
        if (selectionResult.shouldRespawn) {
            spawnSystem.SetCellFactory(*selectionResult.cellFactory);
            _stats.generation = selectionResult.generation;
            respawn();
        }
    }

    _stats.cellsCount = idSystem.GetCellsCount();
    _stats.tick += 1;

    return {};
}

void SimulationScript::SetParameters(const SimulationParameters& parameters)
{
    _parameters = parameters;
    switch (_parameters.spawnPolicy) {
    case SimulationParameters::SpawnPolicy::Random:
        _cellFactory = _randomCellFactory.get();
        break;
    case SimulationParameters::SpawnPolicy::Patrol:
        _cellFactory = _patrolCellFactory.get();
        break;
    default:
        UNREACHABLE("Unknown spawn policy!", _parameters.spawnPolicy);
    }
}
