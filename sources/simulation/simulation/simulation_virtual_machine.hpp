#pragma once

template <class Procedure, class... Args>
    requires std::constructible_from<Procedure, Args...>
void SimulationVirtualMachine::RegisterProcedure(ProcedureType type, uint8_t inputCount, uint8_t outputCount, std::string name, Args&&... args)
{
    // Register procedure in virtual machine
    auto procedure = std::make_unique<Procedure>(std::forward<Args>(args)...);
    const auto procedureId = _virtualMachine.RegisterProcedure(procedure.get(), inputCount, outputCount);
    ASSERT(procedureId != ProcedureId::Invalid);

    {
        // Register procedure info
        const auto index = static_cast<uint16_t>(procedureId);
        ASSERT(_procedureDataList[index].procedure == nullptr, "Another procedure registered by the same index!", procedureId);

        SimulationProcedureInfo info { std::move(name), inputCount, outputCount, type };
        _procedureDataList[index] = ProcedureData { std::move(info), std::move(procedure) };
    }

    {
        // Register procedure type
        const auto index = static_cast<uint8_t>(type);
        ASSERT(_procedureTypeMapping[index] == ProcedureId::Invalid, "Another procedure id bound with the same type!", procedureId);
        _procedureTypeMapping[index] = procedureId;
    }
}