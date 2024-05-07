#include "simulation_virtual_machine.h"
#include "storage.h"
#include "systems/brain_system.h"

SimulationVirtualMachine::SimulationVirtualMachine(Config&& config)
    : _virtualMachine(std::move(config.processorStateWatcher), config.systemInstructionPerStep)
    , _brainSystem(&config.systems.Modify<BrainSystem>())
    , _procedureDataList(ProcedureTableLimit)
    , _procedureTypeMapping(ProcedureTableLimit, ProcedureId::Invalid)
{
}

void SimulationVirtualMachine::Run(CellId id)
{
    ProcessorMemory memory = _brainSystem->AccessMemory(id);
    _runningCellId = id;
    _virtualMachine.Run(memory);
    _runningCellId = CellId::Invalid;
}

ProcedureId SimulationVirtualMachine::GetProcedureId(ProcedureType type) const
{
    const auto index = static_cast<uint8_t>(type);
    ASSERT(index < _procedureTypeMapping.size());
    return _procedureTypeMapping[index];
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