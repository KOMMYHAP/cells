#pragma once

#include "cell_factory_interface.h"
#include "processor/processor_instruction.h"
#include "random/random.hpp"
#include "simulation/simulation_virtual_machine.h"

class RandomCellFactory : public ICellFactory {
public:
    RandomCellFactory(const SimulationVirtualMachine& vm, Random::Engine& random);

    Result Make(const Parent& parent) override;

private:
    ProcessorInstruction GenerateProcessorInstruction();

    std::tuple<std::byte, std::byte> GenerateTwoOperand(ProcessorInstruction hint);
    std::byte GenerateOneOperand(ProcessorInstruction hint);
    std::byte GenerateRegistryIndex();
    std::byte GenerateValue();
    std::byte GenerateProcedureId();

    bool InitMemory(ProcessorMemory& memory);

    gsl::not_null<const SimulationVirtualMachine*> _vm;
    gsl::not_null<Random::Engine*> _random;
};