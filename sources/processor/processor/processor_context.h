#pragma once

#include "procedures/procedure_table.h"
#include "processor_control_block.h"
#include "processor_external_context.h"
#include "processor_memory.h"
#include "processor_stack.h"
#include "processor_state.h"

class ProcessorContext {
public:
    struct Params {
        gsl::not_null<const ProcedureTable*> procedureTable;
        gsl::not_null<ProcessorControlBlock*> controlBlock;
        ProcessorExternalContext externalContext;
        ProcessorMemory memory;
    };

    explicit ProcessorContext(Params params);

    bool StartProcedure(ProcedureId id);
    bool CompleteProcedure(ProcedureId id, uint8_t ignoredInputArgs, uint8_t missingOutputArgs);
    bool AbortProcedure(ProcedureId id, ProcessorState state);
    bool HasPendingProcedure() const { return _pendingProcedure != ProcedureId::Invalid; }

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

    const ProcessorExternalContext& GetExternalContext() const { return _params.externalContext; }
    ProcessorExternalContext& ModifyExternalContext() { return _params.externalContext; }

    const ProcessorControlBlock& GetControlBlock() const { return *_params.controlBlock; }
    ProcessorControlBlock& ModifyControlBlock() { return *_params.controlBlock; }

private:
    Params _params;
    ProcessorMemory _initialMemory;
    ProcessorStack _stack;
    ProcedureId _pendingProcedure { ProcedureId::Invalid };
};

#include "processor_context.hpp"
