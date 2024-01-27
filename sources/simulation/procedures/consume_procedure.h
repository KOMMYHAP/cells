#pragma once

#include "procedures/procedure.h"

class SimulationVirtualMachine;

class ConsumeProcedure final : public ProcedureBase {
public:
    ConsumeProcedure(const SimulationVirtualMachine& vm);

    void Execute(ProcedureContext& context) override;

private:
    const SimulationVirtualMachine& _vm;
};