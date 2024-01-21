#pragma once

#include "brain.h"
#include "brain_packed_data.h"
#include "world.h"

enum class ProcessorInstruction : uint8_t;
class Brain;
class Memory;

class BrainProcessor {
public:
    BrainProcessor(CellId cellId, Brain& brain, World& world);

    static Brain MakeDefaultUnit();

    void Process();

private:
    void ProcessCommand();
    void ProcessSystemCommand(BrainControlBlock& controlBlock, Memory brainData, ProcessorInstruction command);
    void ProcessUnitCommand(BrainControlBlock& controlBlock, Memory brainData, UnitCommand command);

    Brain& _brain;
    World& _world;
    const CellId _id;
};