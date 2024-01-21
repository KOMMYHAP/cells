#include "components/cell_id.h"
#include "vm/virtual_machine.h"

class BrainSystem;
class TypeSystem;

class SimulationVirtualMachine {
public:
    SimulationVirtualMachine(BrainSystem& brainSystem, TypeSystem& typeSystem);

    void Run(CellId id);

    CellId GetRunningCellId() const { return _runningCellId; }

private:
    VirtualMachine _virtualMachine;
    BrainSystem& _brainSystem;
    TypeSystem& _typeSystem;
    CellId _runningCellId;
};
