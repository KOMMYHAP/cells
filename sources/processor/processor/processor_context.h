#pragma once

#include "memory.h"
#include "procedures/procedure_table.h"
#include "processor_control_block.h"

class ProcessorContext {
public:
    ProcessorContext(const ProcedureTable& procedureTable, ProcessorControlBlock& controlBlock, const Memory& memory);

    bool RunProcedure(ProcedureId id);

    ConstMemory GetMemory() const;
    template <class... Ts>
    std::tuple<bool, Ts...> TryReadMemory();
    bool SetCommandPointer(uint8_t nextCommand);
    bool MoveCommandPointer(uint8_t offset);

    bool HasFlag(ProcessorFlags flag) const;
    void SetFlag(ProcessorFlags flag);
    void ResetFlag(ProcessorFlags flag);

    bool IsState(ProcessorState state) const;
    void SetState(ProcessorState state);
    void SetStateWatcher(std::function<void(ProcessorState)> watcher);

    bool WriteRegistry(uint8_t index, std::byte data);
    std::pair<bool, std::byte> ReadRegistry(uint8_t index);

private:
    ProcessorControlBlock& _controlBlock;
    const ProcedureTable& _procedureTable;
    Memory _memory;
    std::function<void(ProcessorState)> _watcher;
};

#include "processor_context.hpp"