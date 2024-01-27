#include "simulation_virtual_machine.h"
#include "components/cell_health.h"
#include "systems/brain_system.h"
#include "systems/health_system.h"
#include "systems/type_system.h"

SimulationVirtualMachine::SimulationVirtualMachine(Config&& config)
    : _virtualMachine(std::move(config.processorStateWatcher), config.systemInstructionPerStep)
    , _brainSystem(config.brainSystem)
    , _typeSystem(config.typeSystem)
    , _procedureDataList(ProcedureTableLimit)
    , _procedureTypeMapping(ProcedureTableLimit, ProcedureId::Invalid)
    , _healthSystem(config.healthSystem)
{
}

void SimulationVirtualMachine::Run(CellId id)
{
    ProcessorMemory memory = _brainSystem.AccessMemory(id);
    _runningCellId = id;
    _virtualMachine.Run(memory);
    _runningCellId = CellId::Invalid;
}

ProcedureId SimulationVirtualMachine::GetProcedureId(ProcedureType type) const
{
    const auto index = static_cast<uint8_t>(type);
    assert(index < _procedureTypeMapping.size());
    return _procedureTypeMapping[index];
}

const SimulationProcedureInfo* SimulationVirtualMachine::FindProcedureInfo(ProcedureType type) const
{
    const ProcedureId procedureId = GetProcedureId(type);
    const auto id = static_cast<uint8_t>(procedureId);
    if (id >= _procedureDataList.size()) {
        // ProcedureType reaches ProcedureTableLimit
        assert(false);
        return nullptr;
    }

    auto& info = _procedureDataList[id];
    if (info.procedure == nullptr) {
        // ProcedureType was not registered.
        return nullptr;
    }

    return &info.info;
}