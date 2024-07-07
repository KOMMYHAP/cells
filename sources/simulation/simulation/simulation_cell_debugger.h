#pragma once
#include "systems_ecs/simulation_ecs_config.h"

#include "processor/processor_control_block.h"
#include "processor/processor_debugger.h"

class SimulationCellDebugger final : public ProcessorDebugger {
public:
    explicit SimulationCellDebugger(EcsWorld& world);

    void SetWatchingCell(CellId id);

    bool ShouldAttachDebugger(const ProcessorContext& context) const override;
    void AttachDebugger(ProcessorContext& context) override;
    void DetachDebugger(ProcessorContext& context) override;

private:
    CellId _watchingCell = CellId::Invalid;
    ProcessorControlBlock _initialControlBlock {};
    gsl::not_null<EcsWorld*> _world;
};