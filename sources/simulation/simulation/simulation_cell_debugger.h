#pragma once
#include "systems_ecs/simulation_ecs_config.h"

#include "processor/processor_control_block.h"
#include "processor/processor_debugger.h"

class SimulationCellDebugger final : public ProcessorDebugger {
public:
    explicit SimulationCellDebugger(EcsWorld& world);

    bool ShouldAttachDebugger(const ProcessorContext& context) const override;
    void AttachDebugger(ProcessorContext& context) override;
    void DetachDebugger(ProcessorContext& context) override;

    void ProcedureWillStarted(ProcessorContext& processorContext, ProcedureContext& procedureContext) override;
    void ProcedureWasDeferred(ProcessorContext& processorContext, const ProcedureContext& procedureContext) override;
    void ProcedureWasCompleted(ProcessorContext& processorContext, const ProcedureContext& procedureContext) override;

private:
    gsl::not_null<EcsWorld*> _world;
};