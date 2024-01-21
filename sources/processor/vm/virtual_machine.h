#pragma once

#include "procedures/procedure.h"
#include "procedures/procedure_table.h"

class VirtualMachine {
public:
    VirtualMachine(uint8_t systemInstructionPerStep);

    bool RegisterProcedure(std::unique_ptr<ProcedureBase> procedure, uint8_t inputArgs, uint8_t outputArgs);

    void Run(Memory memory);

private:
    std::vector<std::unique_ptr<ProcedureBase>> _procedureOwner;
    ProcedureTable _procedureTable;
    uint8_t _systemInstructionPerStep;

};