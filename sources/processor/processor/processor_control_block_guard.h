#pragma once
#include "processor_control_block.h"

class ProcessorControlBlockGuard {
public:
    ProcessorControlBlockGuard(ProcessorControlBlock& controlBlock);
    ProcessorControlBlockGuard(const ProcessorControlBlockGuard&) = delete;
    ProcessorControlBlockGuard(ProcessorControlBlockGuard&&) = delete;
    ProcessorControlBlockGuard& operator=(const ProcessorControlBlockGuard&) = delete;
    ProcessorControlBlockGuard& operator=(ProcessorControlBlockGuard&&) = delete;
    ~ProcessorControlBlockGuard();

    bool ShouldRollback() const { return _shouldRollback; }
    void Submit();

private:
    bool _shouldRollback { true };
    gsl::not_null<ProcessorControlBlock*> _controlBlock;
    ProcessorControlBlock _initialControlBlock;
};