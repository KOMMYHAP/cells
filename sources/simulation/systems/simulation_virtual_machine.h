#pragma once

#include "components/cell_id.h"
#include "components/procedure_type.h"
#include "vm/virtual_machine.h"

class BrainSystem;
class HealthSystem;
class TypeSystem;

struct SimulationProcedureInfo {
    std::string name;
    uint8_t inputArgsCount { 0 };
    uint8_t outputArgsCount { 0 };
    ProcedureType type;
};

class SimulationVirtualMachine {
public:
    struct Config {
        BrainSystem& brainSystem;
        TypeSystem& typeSystem;
        HealthSystem& healthSystem;
        uint8_t systemInstructionPerStep;
        ProcessorStateWatcher processorStateWatcher;
    };

    SimulationVirtualMachine(Config&& config);

    template <class Procedure, class... Args>
        requires std::constructible_from<Procedure, Args...>
    void RegisterProcedure(ProcedureType type, uint8_t inputCount, uint8_t outputCount, std::string name, Args&&... args);

    void Run(CellId id);

    CellId GetRunningCellId() const { return _runningCellId; }
    ProcedureId GetProcedureId(ProcedureType type) const;
    const SimulationProcedureInfo* FindProcedureInfo(ProcedureType type) const;

private:
    struct ProcedureData {
        SimulationProcedureInfo info;
        std::unique_ptr<ProcedureBase> procedure;
    };

    VirtualMachine _virtualMachine;
    BrainSystem& _brainSystem;
    TypeSystem& _typeSystem;
    HealthSystem& _healthSystem;
    CellId _runningCellId;
    std::vector<ProcedureData> _procedureDataList;
    std::vector<ProcedureId> _procedureTypeMapping;
};

#include "simulation_virtual_machine.hpp"