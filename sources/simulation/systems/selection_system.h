#pragma once

#include "cell_factories/preset_cell_factory.h"
#include "components/cell_brain.h"

class BrainSystem;
class IdSystem;

class SelectionSystem {
public:
    SelectionSystem(BrainSystem& brainSystem, IdSystem& idSystem, uint32_t waitingForTicks, uint32_t bestCellsLimit);

    struct Result {
        bool shouldRespawn { false };
        ICellFactory* cellFactory { nullptr };
        const std::vector<CellBrain>* parentBrains { nullptr };
        uint32_t generation { 0 };
    };
    Result TickGeneration();

    void Restart();

private:
    void MakeNextGeneration();

    std::vector<CellBrain> CollectBestBrains() const;
    void AddHistoryRecord(std::vector<CellBrain> brains);

    uint32_t _currentTick { 0 };
    uint32_t _currentGeneration { 0 };
    const uint32_t _waitingForTicks { 0 };

    BrainSystem& _brainSystem;
    IdSystem& _idSystem;

    struct BrainSelection {
        std::vector<CellBrain> brains;
        uint32_t generation;
    };
    std::vector<BrainSelection> _selectionHistory;
    PresetCellFactory _selectionPresetFactory;
    const uint32_t _bestCellsLimit { 0 };
};
