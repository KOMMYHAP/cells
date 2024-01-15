#pragma once

#include "../field/field.h"
#include "brain_packed_data.h"
#include "cell.h"

enum class ProcessorInstruction : uint8_t;
class Brain;
class Memory;

class BrainProcessor {
public:
    BrainProcessor(CellId cellId, Brain& brain, Field& field);

    static Cell MakeDefaultUnit();

    void Process();

private:
    void ProcessCommand();
    void ProcessSystemCommand(BrainControlBlock& controlBlock, Memory brainData, ProcessorInstruction command);
    void ProcessUnitCommand(BrainControlBlock& controlBlock, Memory brainData, UnitCommand command);

    Brain& _brain;
    Field& _field;
    const CellId _cellId;
};