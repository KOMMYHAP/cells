#include "simulation_cell_debugger.h"

#include "procedures/simulation_procedure_context.h"
#include "processor/processor_context.h"

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
}