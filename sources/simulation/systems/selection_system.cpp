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
    std::vector<CellBrain> parentBrains = CollectBestBrains();
    const uint32_t bestCellsCount = parentBrains.size();

    // make descendants of best cells
    constexpr uint8_t crossoverPoint = 10;
    CrossoverAlgorithm crossover { crossoverPoint };
    std::vector<CellBrain> childBrains;
    childBrains.reserve(bestCellsCount * (bestCellsCount - 1) / 2 + 1);
    for (uint32_t i = 0; i < bestCellsCount; ++i) {
        for (uint32_t j = i + 1; j < bestCellsCount; ++j) {
            const CellBrain& lhsBrain = parentBrains[i];
            const CellBrain& rhsBrain = parentBrains[j];
            const CellBrain childBrain = crossover.Combine(lhsBrain, rhsBrain);
            childBrains.emplace_back(childBrain);
        }
    }

    _selectionPresetFactory.SetPreset(std::move(childBrains));
    AddHistoryRecord(std::move(parentBrains));
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
    std::vector<CellBrain> parentBrains;
    parentBrains.reserve(_bestCellsLimit);

    std::vector<CellId> aliveCellIds;
    aliveCellIds.reserve(_idSystem.GetCellsCount());
    _idSystem.Iterate([&](const CellId id) {
        aliveCellIds.push_back(id);
    });

    std::shuffle(aliveCellIds.begin(), aliveCellIds.end(), common::GetRandomEngine());
    const uint16_t bestCellsCount = std::min<uint16_t>(_bestCellsLimit, aliveCellIds.size());

    std::ranges::transform(std::span(aliveCellIds).first(bestCellsCount), std::back_inserter(parentBrains), [&](const CellId id) {
        return _brainSystem.Get(id);
    });

    if (bestCellsCount < _bestCellsLimit) {
        std::vector<CellBrain> copies = parentBrains;
        const uint32_t fullCopies = (_bestCellsLimit - bestCellsCount) / copies.size();
        for (uint32_t copyIndex = 0; copyIndex < fullCopies; ++copyIndex) {
            parentBrains.insert(parentBrains.end(), copies.begin(), copies.end());
        }
        const uint32_t restCopies = (_bestCellsLimit - bestCellsCount) - fullCopies * bestCellsCount;
        assert(restCopies < copies.size());
        for (uint32_t i = 0; i < restCopies; ++i) {
            parentBrains.emplace_back(copies[i]);
        }
    }

    std::uniform_int_distribution<size_t> mutation { 0, parentBrains.size() - 1 };
    const size_t mutationIndex = mutation(common::GetRandomEngine());
    CellBrain& brain = parentBrains[mutationIndex];
    ProcessorMemory memory { brain.data };

    assert(static_cast<uint16_t>(memory.Size() >= sizeof(ProcessorControlBlock) + 1));
    std::uniform_int_distribution<uint16_t> brainMutation { 0, static_cast<uint16_t>(memory.Size() - sizeof(ProcessorControlBlock) - 1) };
    const uint8_t brainMutationIndex = static_cast<uint8_t>(brainMutation(common::GetRandomEngine()));
    memory.Move(brainMutationIndex + sizeof(ProcessorControlBlock));

    std::uniform_int_distribution<uint16_t> dataMutation { 0, 255 };
    const uint8_t data = static_cast<uint8_t>(dataMutation(common::GetRandomEngine()));
    memory.Write(data);

    return parentBrains;
}

void SelectionSystem::AddHistoryRecord(std::vector<CellBrain> brains)
{
    BrainSelection& historyRecord = _selectionHistory.emplace_back();
    historyRecord.brains = std::move(brains);
    historyRecord.generation = _currentGeneration;
}
