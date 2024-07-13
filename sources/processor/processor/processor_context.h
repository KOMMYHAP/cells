#pragma once

#include "procedures/procedure_context.h"
#include "procedures/procedure_external_context.h"
#include "procedures/procedure_id.h"
#include "processor_control_block.h"
#include "processor_memory.h"
#include "processor_stack.h"
#include "processor_state.h"

class ProcedureContext;
class ProcedureTable;

class ProcessorContext {
public:
    struct Params {
        gsl::not_null<const ProcedureTable*> procedureTable;
        gsl::not_null<ProcessorControlBlock*> controlBlock;
        ProcedureExternalContext externalContext;
        ProcessorMemory memory;
    };

    explicit ProcessorContext(Params params);

    bool StartProcedure(ProcedureId id);
    bool CompletePendingProcedure(const ProcedureContext& context);
    bool HasPendingProcedure() const { return _pendingProcedureId != ProcedureId::Invalid; }

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

private:
    Params _params;
    ProcessorMemory _initialMemory;
    ProcessorStack _stack;
    ProcedureId _pendingProcedureId { ProcedureId::Invalid };
};

#include "processor_context.hpp"
