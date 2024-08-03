#include "simulation_cell_debugger.h"

#include "processor/processor_context.h"
#include "simulation_procedure_context.h"

SimulationCellDebugger::SimulationCellDebugger(EcsWorld& world)
    : _world(&world)
{
}

bool SimulationCellDebugger::ShouldAttachDebugger(const ProcessorContext& context) const
{
    return true;
}

void SimulationCellDebugger::AttachDebugger(ProcessorContext& context)
{
}

void SimulationCellDebugger::DetachDebugger(ProcessorContext& context)
{
    if (context.IsState(ProcessorState::Good) || context.IsState(ProcessorState::PendingProcedure)) {
        return;
    }

    const CellId id = context.GetUserData().Get<SimulationProcedureContext>().id;
    _world->destroy(id);
}

void SimulationCellDebugger::ProcedureWillStarted(ProcessorContext& processorContext, ProcedureContext& procedureContext)
{
}

void SimulationCellDebugger::ProcedureWasDeferred(ProcessorContext& processorContext, const ProcedureContext& procedureContext)
{
}

void SimulationCellDebugger::ProcedureWasCompleted(ProcessorContext& processorContext, const ProcedureContext& procedureContext)
{
}