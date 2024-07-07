#pragma once

#include "processor/processor_control_block.h"
#include "processor/processor_debugger.h"

class SimulationCellDebugger final : public ProcessorDebugger {
public:
    void SetWatchingCell(CellId id);

    bool ShouldAttachDebugger(const ProcessorContext& context) const override;
    void AttachDebugger(ProcessorContext& context) override;
    void DetachDebugger(ProcessorContext& context) override;

private:
    CellId _watchingCell = CellId::Invalid;
    ProcessorControlBlock _initialControlBlock {};
};