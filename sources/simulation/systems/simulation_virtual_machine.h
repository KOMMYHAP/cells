#pragma once

#include "components/cell_id.h"
#include "components/procedure_type.h"
#include "vm/virtual_machine.h"

class BrainSystem;
class HealthSystem;
class TypeSystem;
class World;

struct SimulationProcedureInfo {
    std::string name;
    uint8_t inputArgsCount { 0 };
    uint8_t outputArgsCount { 0 };
    ProcedureType type;
};

class SimulationVirtualMachine {
public:
    SimulationVirtualMachine(BrainSystem& brainSystem, TypeSystem& typeSystem, HealthSystem& healthSystem);

    void CreateProcedures(World& world);
    void Run(CellId id);

    CellId GetRunningCellId() const { return _runningCellId; }
    ProcedureId GetProcedureId(ProcedureType type) const;
    const SimulationProcedureInfo* FindProcedureInfo(ProcedureType type) const;

private:
    struct Impl;

    struct ProcedureData {
        SimulationProcedureInfo info;
        std::unique_ptr<ProcedureBase> procedure;
    };

    static ProcessorStateWatcher MakeSimulationWatcher(SimulationVirtualMachine* simulationVm);

    VirtualMachine _virtualMachine;
    BrainSystem& _brainSystem;
    TypeSystem& _typeSystem;
    HealthSystem& _healthSystem;
    CellId _runningCellId;
    std::vector<ProcedureData> _procedureDataList;
};
