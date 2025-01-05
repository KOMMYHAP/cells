#include "simulation_cell_debugger.h"

#include "processor/processor_context.h"
#include "simulation_procedure_context.h"

SimulationCellDebugger::SimulationCellDebugger(EcsWorld& world)
    : _world(&world)
{
}

bool SimulationCellDebugger::ShouldAttachDebugger(const ProcessorContext& /*context*/) const
{
    return false;
}

void SimulationCellDebugger::AttachDebugger(ProcessorContext& /*context*/)
{
}

void SimulationCellDebugger::DetachDebugger(ProcessorContext& /*context*/)
{
}

void SimulationCellDebugger::ProcedureWillStarted(ProcessorContext& /*processorContext*/, ProcedureContext& /*procedureContext*/)
{
}

void SimulationCellDebugger::ProcedureWasDeferred(ProcessorContext& /*processorContext*/, const ProcedureContext& /*procedureContext*/)
{
}

void SimulationCellDebugger::ProcedureWasCompleted(ProcessorContext& /*processorContext*/, const ProcedureContext& /*procedureContext*/)
{
}