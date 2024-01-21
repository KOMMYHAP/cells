#pragma once
#include "processor_control_block.h"

class ProcessorControlBlockGuard {
public:
    ProcessorControlBlockGuard(ProcessorControlBlock& controlBlock);
    ~ProcessorControlBlockGuard();

    void Submit();

private:
    bool _shouldRollback { true };
    ProcessorControlBlock& _controlBlock;
    ProcessorControlBlock _initialControlBlock;
};