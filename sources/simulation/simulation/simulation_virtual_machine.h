#pragma once

#include "components/cell_id.h"
#include "procedures/procedure_type.h"
#include "simulation_cell_debugger.h"
#include "vm/virtual_machine.h"

struct CellBrain;
class BrainSystem;

struct SimulationProcedureInfo {
    std::string name;
    uint8_t inputArgsCount { 0 };
    uint8_t outputArgsCount { 0 };
    ProcedureType type { ProcedureType::LastProcedureType };
};

class SimulationVirtualMachine {
public:
    SimulationVirtualMachine();

    template <class Procedure, class... Args>
        requires std::constructible_from<Procedure, Args...>
    void RegisterProcedure(ProcedureType type, uint8_t inputCount, uint8_t outputCount, std::string name, Args&&... args);

    SimulationCellDebugger& ModifyCellDebugger() { return _debugger; }

    void Run(CellId id, CellBrain& brain);
    ProcedureContext RestoreDeferredExecution(CellId id, CellBrain& brain);

    ProcedureId GetProcedureId(ProcedureType type) const;
    const SimulationProcedureInfo* FindProcedureInfo(ProcedureType type) const;

private:
    struct ProcedureData {
        SimulationProcedureInfo info;
        std::unique_ptr<ProcedureBase> procedure;
    };

    VirtualMachine _virtualMachine;
    SimulationCellDebugger _debugger;
    std::vector<ProcedureData> _procedureDataList;
    std::vector<ProcedureId> _procedureTypeMapping;
};

#include "simulation_virtual_machine.hpp"