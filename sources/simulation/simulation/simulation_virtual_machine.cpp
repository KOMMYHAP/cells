#include "simulation_virtual_machine.h"

#include "procedures/procedure_context.h"
#include "simulation_procedure_context.h"
#include "storage/storage.h"
#include "systems/brain_system.h"

SimulationVirtualMachine::SimulationVirtualMachine(EcsWorld& world)
    : _debugger(world)
    , _procedureDataList(ProcedureTableLimit)
    , _procedureTypeMapping(ProcedureTableLimit, ProcedureId::Invalid)
{
    _virtualMachine.SetDebugger(&_debugger);
}

void SimulationVirtualMachine::Run(const CellId id, CellBrain& brain)
{
    const ProcessorMemory memory { brain.data };
    _virtualMachine.Run(memory, std::make_any<SimulationProcedureContext>(id));
}

ProcedureContext SimulationVirtualMachine::RestoreDeferredExecution(const CellId id, CellBrain& brain)
{
    const ProcessorMemory memory { brain.data };
    const ProcedureContext context = _virtualMachine.RestoreDeferredExecution(memory);
    ASSERT(context.GetExternalContext<SimulationProcedureContext>().id == id);
    return context;
}

ProcedureId SimulationVirtualMachine::GetProcedureId(ProcedureType type) const
{
    const auto index = static_cast<uint8_t>(type);
    ASSERT(index < _procedureTypeMapping.size());
    const ProcedureId id = _procedureTypeMapping[index];
    ASSERT(id != ProcedureId::Invalid, "Procedure was not registered!");
    return id;
}

const SimulationProcedureInfo* SimulationVirtualMachine::FindProcedureInfo(ProcedureType type) const
{
    const ProcedureId procedureId = GetProcedureId(type);
    const auto id = static_cast<uint8_t>(procedureId);
    ASSERT(id < _procedureDataList.size());

    auto& info = _procedureDataList[id];
    if (info.procedure == nullptr) {
        // ProcedureType was not registered.
        return nullptr;
    }

    return &info.info;
}
