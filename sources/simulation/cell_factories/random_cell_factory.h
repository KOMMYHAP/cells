#pragma once

#include "cell_factory_interface.h"

class ProcessorMemory;
class SimulationVirtualMachine;
enum class ProcessorInstruction : uint8_t;
enum class ProcedureType : uint8_t;

class RandomCellFactory : public ICellFactory {
public:
    RandomCellFactory(SimulationVirtualMachine& vm, std::optional<uint16_t> limitBrainSize = {});

    Result Make() override;

private:
    ProcessorInstruction GenerateProcessorInstruction();

    std::tuple<std::byte, std::byte> GenerateTwoOperand(ProcessorInstruction hint);
    std::byte GenerateOneOperand(ProcessorInstruction hint);
    std::byte GenerateRegistryIndex();
    std::byte GenerateValue();
    std::byte GenerateProcedureArgs(ProcedureType hint);
    std::byte GenerateProcedureId();

    bool InitMemory(ProcessorMemory& memory);

    uint16_t _brainSize { 0 };
    SimulationVirtualMachine& _vm;
};
