#include "simulation_cell_debugger.h"

#include "processor/processor_context.h"
#include "simulation_procedure_context.h"

SimulationCellDebugger::SimulationCellDebugger(EcsWorld& world)
    : _world(&world)
{
}

void SimulationCellDebugger::SetWatchingCell(CellId id)
{
    _watchingCell = id;
}

bool SimulationCellDebugger::ShouldAttachDebugger(const ProcessorContext& context) const
{
    return context.GetExternalContext().Get<SimulationProcedureContext>().id == _watchingCell;
}

void SimulationCellDebugger::AttachDebugger(ProcessorContext& context)
{
    _initialControlBlock = context.GetControlBlock();
}

void SimulationCellDebugger::DetachDebugger(ProcessorContext& context)
{
    if (context.IsState(ProcessorState::Good)) {
        return;
    }

    context.ModifyControlBlock() = _initialControlBlock;

    const CellId id = context.GetExternalContext().Get<SimulationProcedureContext>().id;
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