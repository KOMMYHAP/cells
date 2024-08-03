// #pragma once
//
// #include "cell_factory_interface.h"
// #include "components/cell_id.h"
//
// class SimulationVirtualMachine;
// class BrainSystem;
// class ProcessorMemory;
//
// class PatrolCellFactory : public ICellFactory {
// public:
//     PatrolCellFactory(SimulationVirtualMachine& simulationVm, uint8_t moveCommandCount);
//
//     Result Make() override;
//
// private:
//     bool InitMemory(ProcessorMemory& memory);
//
//     uint8_t _moveCommandCount { 0 };
//     SimulationVirtualMachine& _simulationVm;
// };
