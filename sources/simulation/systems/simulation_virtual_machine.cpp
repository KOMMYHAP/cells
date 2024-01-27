#include "simulation_virtual_machine.h"
#include <procedures/move_procedure.h>

#include "brain_system.h"
#include "type_system.h"
#include "world.h"

constexpr uint8_t SystemInstructionPerStep { 8 };

struct SimulationVirtualMachine::Impl {
    SimulationVirtualMachine& super;

    template <class Procedure, class... Args>
    void RegisterProcedure(ProcedureType type, uint8_t inputCount, uint8_t outputCount, std::string name, Args&&... args)
    {
        auto procedure = std::make_unique<Procedure>(std::forward<Args>(args)...);
        const auto procedureId = super._virtualMachine.RegisterProcedure(procedure.get(), inputCount, outputCount);
        if (procedureId == ProcedureId::Invalid) {
            assert(false);
            return;
        }

        const auto index = static_cast<uint16_t>(procedureId);
        assert(super._procedureDataList[index].procedure == nullptr);

        SimulationProcedureInfo info { std::move(name), inputCount, outputCount, type };
        super._procedureDataList[index] = ProcedureData { std::move(info), std::move(procedure) };
    }
};

SimulationVirtualMachine::SimulationVirtualMachine(BrainSystem& brainSystem, TypeSystem& typeSystem, HealthSystem& healthSystem)
    : _virtualMachine(MakeSimulationWatcher(this), SystemInstructionPerStep)
    , _brainSystem(brainSystem)
    , _typeSystem(typeSystem)
    , _procedureDataList(ProcedureTableLimit)
    , _healthSystem(healthSystem)
{
}

void SimulationVirtualMachine::Run(CellId id)
{
    const CellType type = _typeSystem.Get(id);
    if (type != CellType::Unit) {
        return;
    }

    ProcessorMemory memory = _brainSystem.AccessMemory(id);
    _runningCellId = id;
    _virtualMachine.Run(memory);
    _runningCellId = CellId::Invalid;
}

void SimulationVirtualMachine::CreateProcedures(World& world)
{
    Impl impl { *this };

    // Please, if you register new command, follow the declaration order of ProcedureType.
    // See more in SimulationVirtualMachine::GetProcedureId.

    impl.RegisterProcedure<MoveProcedure>(ProcedureType::Move, 1, 0, "move", world.virtualMachine, world.positionSystem);
}

ProcedureId SimulationVirtualMachine::GetProcedureId(ProcedureType type) const
{
    // A little hack here: we suppose that ProcedureId maps to ProcedureType exactly (1 to 1) to O(1) access to procedure info.
    return static_cast<ProcedureId>(type);
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

ProcessorStateWatcher SimulationVirtualMachine::MakeSimulationWatcher(SimulationVirtualMachine* simulationVm)
{
    return [simulationVm](ProcessorState state) {
        if (state == ProcessorState::Good) {
            return;
        }

        // Cell's brain has illegal instruction, make insult as punishment
        const CellId id = simulationVm->_runningCellId;
        simulationVm->_healthSystem.Set(id, CellHealth::Zero);
    };
}
