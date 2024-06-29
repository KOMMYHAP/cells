#pragma once

#include "procedures/procedure_table.h"
#include "processor_control_block.h"
#include "processor_control_block_guard.h"
#include "processor_external_context.h"
#include "processor_memory.h"
#include "processor_state.h"

class ProcessorContext {
public:
    struct Params {
        gsl::not_null<const ProcedureTable*> procedureTable;
        gsl::not_null<const ProcessorStateWatcher*> stateWatcher;
        gsl::not_null<ProcessorControlBlock*> controlBlock;
        gsl::not_null<ProcessorExternalContext*> externalContext;
        ProcessorMemory memory;
    };

    explicit ProcessorContext(Params params);

    ProcessorControlBlockGuard MakeGuard();
    bool RunProcedure(ProcedureId id);

    template <class... Ts>
    std::tuple<bool, Ts...> TryReadMemory();

    bool SetCommandPointer(uint8_t nextCommand);
    bool MoveCommandPointer(uint8_t offset);

    bool HasFlag(ProcessorFlags flag) const;
    void SetFlag(ProcessorFlags flag, bool value);
    void SetFlag(ProcessorFlags flag);
    void ResetFlag(ProcessorFlags flag);

    bool IsState(ProcessorState state) const;
    void SetState(ProcessorState state);

    bool WriteRegistry(uint8_t index, std::byte data);
    std::pair<bool, std::byte> ReadRegistry(uint8_t index);
    bool PushStack(std::byte data);
    std::pair<bool, std::byte> PopStack();

    const ProcessorExternalContext& GetExternalContext() const { return *_params.externalContext; }
    ProcessorExternalContext& ModifyExternalContext() { return *_params.externalContext; }

private:
    Params _params;
    ProcessorMemory _initialMemory;
    ProcessorStack _stack;
};

#include "processor_context.hpp"
