#pragma once

#include "brain_packed_data.h"
#include "cell.h"
#include "field.h"

class Brain;
class BrainData;

class UnitProcessor {
public:
    UnitProcessor(CellId cellId, Brain& brain, Field& field);

    static Cell MakeDefaultUnit();

    void Process();

private:
    void ProcessCommand();
    void ProcessSystemCommand(UnitControlBlock& controlBlock, BrainData brainData, SystemCommand command);
    void ProcessUnitCommand(UnitControlBlock& controlBlock, BrainData brainData, UnitCommand command);

    Brain& _brain;
    Field& _field;
    const CellId _cellId;
};