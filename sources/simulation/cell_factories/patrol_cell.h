#pragma once
#include "cell_factory_interface.h"
#include "simulation/simulation_virtual_machine.h"

class PatrolCellFactory final : public ICellFactory {
public:
    explicit PatrolCellFactory(const SimulationVirtualMachine& vm);
    Result Make(const Parent& parent) override;

private:
    gsl::not_null<const SimulationVirtualMachine*> _vm;
};