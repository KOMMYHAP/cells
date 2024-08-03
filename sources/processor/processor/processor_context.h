#pragma once

#include "procedures/procedure.h"
#include "procedures/procedure_context.h"
#include "procedures/procedure_id.h"
#include "procedures/procedure_table.h"
#include "processor_control_block.h"
#include "processor_memory.h"
#include "processor_state.h"

class ProcessorContext {
public:
    struct Params {
        gsl::not_null<const ProcedureTable*> procedureTable;
        gsl::not_null<ProcessorControlBlock*> controlBlock;
        ProcessorUserData userData;
        ProcessorMemory memory;
    };

    explicit ProcessorContext(Params params);

    bool HasPendingProcedure() const;
    void SetPendingProcedure(ProcedureId id);
    ProcedureId GetPendingProcedure() const;

    template <class... Ts>
    std::tuple<bool, Ts...> TryReadMemory();

    bool SetCommandPointer(uint8_t nextCommand);
    bool MoveCommandPointer(uint8_t offset);

    bool HasFlag(ProcessorFlags flag) const;
    void SetFlag(ProcessorFlags flag, bool value);
    void SetFlag(ProcessorFlags flag);
    void ResetFlag(ProcessorFlags flag);

    bool IsState(ProcessorState state) const;
    ProcessorState GetState() const;
    void SetState(ProcessorState state);

    bool WriteRegistry(uint8_t index, std::byte data);
    std::pair<bool, std::byte> ReadRegistry(uint8_t index);
    bool PushStack(std::byte data);
    std::pair<bool, std::byte> PopStack();

    const ProcessorControlBlock& GetControlBlock() const { return *_params.controlBlock; }
    ProcessorControlBlock& ModifyControlBlock() { return *_params.controlBlock; }

    const ProcessorUserData& GetUserData() const { return _params.userData; }
    ProcessorUserData& ModifyUserData() { return _params.userData; }

    std::optional<ProcedureContext> MakeProcedureContext(ProcedureId id) const;
    ProcedureBase& GetProcedure(ProcedureId id);

private:
    Params _params;
    ProcessorMemory _initialMemory;
};

#include "processor_context.hpp"
