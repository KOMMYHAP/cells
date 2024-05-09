#pragma once

#include "base_script.h"
#include "cell_factories/cell_factory_interface.h"
#include "cell_factories/patrol_cell_factory.h"
#include "cell_factories/random_cell_factory.h"
#include "components/cell_age.h"

struct SimulationParameters {
    enum class SpawnPolicy {
        Random,
        Patrol
    };

    CellAge limitCellAge { 100 };
    uint16_t bestCellSelectionSize { 100 };
    uint16_t selectionEpochTicks { 1000 };
    SpawnPolicy spawnPolicy { SpawnPolicy::Random };
};

struct SimulationStatistics {
    uint32_t cellsCount { 0 };
    uint32_t generation { 0 };
    uint64_t tick { 0 };
};

class SimulationScript final : public BaseScript {
public:
    SimulationScript(const common::Storage& systems);

    std::expected<void, std::error_code> Perform() override;

    void SetParameters(const SimulationParameters& parameters);
    const SimulationStatistics& GetStatistics() const { return _stats; }

private:
    const common::Storage& _systems;
    SimulationParameters _parameters;
    SimulationStatistics _stats;
    std::unique_ptr<PatrolCellFactory> _patrolCellFactory;
    std::unique_ptr<RandomCellFactory> _randomCellFactory;
    ICellFactory* _cellFactory { nullptr };
};