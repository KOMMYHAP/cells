#pragma once
#include "processor_control_block.h"

class ProcessorContext;
class ProcessorControlBlockGuard {
public:
    ProcessorControlBlockGuard(ProcessorContext& context);
    ProcessorControlBlockGuard(const ProcessorControlBlockGuard&) = delete;
    ProcessorControlBlockGuard& operator=(const ProcessorControlBlockGuard&) = delete;
    ProcessorControlBlockGuard(ProcessorControlBlockGuard&& other) noexcept;
    ProcessorControlBlockGuard& operator=(ProcessorControlBlockGuard&& other) noexcept;
    ~ProcessorControlBlockGuard();

    void Swap(ProcessorControlBlockGuard& other) noexcept;

    void Submit();

private:
    bool _shouldRollback { true };
    gsl::not_null<ProcessorContext*> _context;
    ProcessorControlBlock _initialControlBlock;
};