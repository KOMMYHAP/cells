#pragma once

#include "procedures/procedure.h"
#include "procedures/procedure_table.h"

class VirtualMachine {
public:
    bool RegisterProcedure(std::unique_ptr<ProcedureBase> procedure, uint8_t inputArgs, uint8_t outputArgs);

    std::optional<ProcessorContext> MakeProcessorContext(Memory memory);

private:
    std::vector<std::unique_ptr<ProcedureBase>> _procedureOwner;
    ProcedureTable _procedureTable;
};