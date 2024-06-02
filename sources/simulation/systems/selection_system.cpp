#include "selection_system.h"
#include "processor/processor_control_block.h"

#include "components/cell_id.h"
#include "genetic/crossover_algorithm.h"
#include "random.h"
#include "systems/brain_system.h"
#include "systems/id_system.h"

SelectionSystem::SelectionSystem(BrainSystem& brainSystem, IdSystem& idSystem, uint32_t waitingForTicks, uint32_t bestCellsLimit)
    : _waitingForTicks(waitingForTicks)
    , _brainSystem(brainSystem)
    , _idSystem(idSystem)
    , _bestCellsLimit(bestCellsLimit)
{
}

SelectionSystem::Result SelectionSystem::TickGeneration()
{
    Result result;
    result.shouldRespawn = false;

    _currentTick = (_currentTick + 1) % _waitingForTicks;
    if (_currentTick != 0) {
        return result;
    }

    MakeNextGeneration();
    BrainSelection& selection = _selectionHistory.back();
    result.generation = selection.generation;
    result.cellFactory = &_selectionPresetFactory;
    result.parentBrains = &selection.brains;
    result.shouldRespawn = true;
    return result;
}

void SelectionSystem::MakeNextGeneration()
{
    // collect best cells
    std::vector<CellBrain> bestBrains = CollectBestBrains();
    _selectionPresetFactory.SetPreset(bestBrains);
    AddHistoryRecord(std::move(bestBrains));
    _currentGeneration += 1;
}

void SelectionSystem::Restart()
{
    _selectionHistory.clear();
    _selectionPresetFactory.SetPreset({});
    _currentTick = 0;
    _currentGeneration = 0;
}

std::vector<CellBrain> SelectionSystem::CollectBestBrains() const
{
    std::vector<CellBrain> bestBrains;
    bestBrains.reserve(_bestCellsLimit);

    std::vector<CellId> aliveCellIds;
    aliveCellIds.reserve(_idSystem.GetCellsCount());
    _idSystem.Iterate([&](const CellId id) {
        aliveCellIds.push_back(id);
    });

    std::ranges::shuffle(aliveCellIds, common::GetRandomEngine());
    const auto bestCellsCount = NarrowCast<uint16_t>(std::min(_bestCellsLimit, NarrowCast<uint32_t>(aliveCellIds.size())));

    std::ranges::transform(std::span(aliveCellIds).first(bestCellsCount), std::back_inserter(bestBrains), [&](const CellId id) {
        return _brainSystem.Get(id);
    });

    return bestBrains;
}

void SelectionSystem::AddHistoryRecord(std::vector<CellBrain> brains)
{
    BrainSelection& historyRecord = _selectionHistory.emplace_back();
    historyRecord.brains = std::move(brains);
    historyRecord.generation = _currentGeneration;
}