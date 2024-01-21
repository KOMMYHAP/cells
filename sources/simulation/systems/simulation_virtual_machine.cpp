#include "simulation_virtual_machine.h"

#include "brain_system.h"
#include "breakpoint.h"
#include "type_system.h"

constexpr uint8_t SystemInstructionPerStep { 8 };

static ProcessorStateWatcher MakeInvalidStateTrap()
{
    return [](ProcessorState state) {
        if (state == ProcessorState::Good) {
            return;
        }
        common::Breakpoint();
    };
}

SimulationVirtualMachine::SimulationVirtualMachine(BrainSystem& brainSystem, TypeSystem& typeSystem)
    : _virtualMachine(MakeInvalidStateTrap(), SystemInstructionPerStep)
    , _brainSystem(brainSystem)
    , _typeSystem(typeSystem)
{
}

void SimulationVirtualMachine::Run(CellId id)
{
    const CellType type = _typeSystem.Get(id);
    if (type != CellType::Unit) {
        return;
    }

    Memory memory = _brainSystem.AccessMemory(id);
    _runningCellId = id;
    _virtualMachine.Run(memory);
    _runningCellId = CellId::Invalid;
}
