#pragma once

#include "brain_packed_data.h"
#include "components/cell_brain.h"
#include "world.h"

enum class ProcessorInstruction : uint8_t;
class CellBrain;
class Memory;

class BrainProcessor {
public:
    BrainProcessor(CellId cellId, CellBrain& brain, World& world);

    static CellBrain MakeDefaultUnit();

    void Process();

private:
    void ProcessCommand();
    void ProcessSystemCommand(BrainControlBlock& controlBlock, Memory brainData, ProcessorInstruction command);
    void ProcessUnitCommand(BrainControlBlock& controlBlock, Memory brainData, UnitCommand command);

    CellBrain& _brain;
    World& _world;
    const CellId _id;
};