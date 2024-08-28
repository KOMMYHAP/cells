#pragma once

template <class Procedure, class... Args>
    requires std::constructible_from<Procedure, Args...>
void SimulationVirtualMachine::RegisterProcedure(ProcedureType type, uint8_t inputCount, uint8_t outputCount, std::string name, Args&&... args)
{
    // Register procedure in virtual machine
    auto procedure = std::make_unique<Procedure>(std::forward<Args>(args)...);
    ProcedureBase* baseProcedure = procedure.get();
    RegisterProcedure(type, baseProcedure, inputCount, outputCount, std::move(name));

    const ProcedureId id = GetProcedureId(type);
    _procedureDataList[static_cast<size_t>(id)].procedureOwner = std::move(procedure);
}