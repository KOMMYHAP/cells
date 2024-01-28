#pragma once

#include "procedures/procedure_table.h"
#include "processor_control_block.h"
#include "processor_control_block_guard.h"
#include "processor_memory.h"
#include "processor_state.h"

class ProcessorContext {
public:
    ProcessorContext(const ProcedureTable& procedureTable, const ProcessorStateWatcher& stateWatcher, ProcessorControlBlock& controlBlock, const ProcessorMemory& memory);

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

private:
    ProcessorControlBlock& _controlBlock;
    const ProcedureTable& _procedureTable;
    const ProcessorMemory _initialMemory;
    ProcessorMemory _memory;
    ProcessorStack _stack;
    const std::function<void(ProcessorState)>& _watcher;
};

#include "processor_context.hpp"
