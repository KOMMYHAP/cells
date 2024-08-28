#pragma once

#include "components/cell_id.h"
#include "procedures/procedure_type.h"
#include "simulation_cell_debugger.h"
#include "vm/virtual_machine.h"

struct CellBrain;
class BrainSystem;

class SimulationVirtualMachine {
public:
    SimulationVirtualMachine(EcsWorld& world);

    template <class Procedure, class... Args>
        requires std::constructible_from<Procedure, Args...>
    void RegisterProcedure(ProcedureType type, uint8_t inputCount, uint8_t outputCount, std::string name, Args&&... args);
    void RegisterProcedure(ProcedureType type, ProcedureBase* procedure, uint8_t inputCount, uint8_t outputCount, std::string name);

    SimulationCellDebugger& ModifyCellDebugger() { return _debugger; }

    void Run(CellId id, CellBrain& brain);
    void CompletePendingProcedure(CellId id, CellBrain& brain, const ProcedureContext& context);

    ProcedureId GetProcedureId(ProcedureType type) const;

    struct SimulationProcedureInfo {
        std::string name;
        uint8_t inputArgsCount { 0 };
        uint8_t outputArgsCount { 0 };
        ProcedureType type { ProcedureType::LastProcedureType };
    };
    const SimulationProcedureInfo* FindProcedureInfo(ProcedureType type) const;

private:
    struct ProcedureData {
        SimulationProcedureInfo info;
        std::unique_ptr<ProcedureBase> procedureOwner;
        ProcedureBase *procedure{nullptr};
    };

    VirtualMachine _virtualMachine;
    SimulationCellDebugger _debugger;
    std::vector<ProcedureData> _procedureDataList;
    std::vector<ProcedureId> _procedureTypeMapping;
};

#include "simulation_virtual_machine.hpp"