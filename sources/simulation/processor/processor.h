#pragma once
#include "procedure.h"
#include "procedure_table.h"
#include "processor_control_block.h"
#include <processor/memory.h>

class ProcessorContext {
public:
    ProcessorContext(const ProcedureTable& procedureTable, ProcessorControlBlock& controlBlock, const Memory& memory);

    void SetInvalidCommandTrap(std::function<void()> trap);

    std::optional<ProcedureContext> MakeProcedureContext(ProcedureId id);

    const Memory& GetMemory() const { return _memory; }
    Memory& AccessMemory() { return _memory; }

    bool HasFlag(ProcessorFlags flag) const;
    void SetFlag(ProcessorFlags flag);
    void ResetFlag(ProcessorFlags flag);

private:
    ProcessorControlBlock& _controlBlock;
    const ProcedureTable& _procedureTable;
    Memory _memory;
    std::function<void()> _invalidCommandTrap;
};

class Processor {
public:
    void Execute(ProcessorContext& context);

private:
    ProcessorContext _context;
};