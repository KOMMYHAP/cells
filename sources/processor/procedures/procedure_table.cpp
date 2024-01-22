#include "procedure_table.h"

ProcedureId ProcedureTable::RegisterProcedure(const ProcedureTableEntry& info)
{
    ProcedureId procedureId = _nextId;
    if (procedureId == ProcedureId::Invalid) {
        return ProcedureId::Invalid;
    }

    if (info.inputArgsCount >= ProcedureInputArgsCountLimit) {
        return ProcedureId::Invalid;
    }
    if (info.outputArgsCount >= ProcedureOutputArgsCountLimit) {
        return ProcedureId::Invalid;
    }

    const auto id = static_cast<uint8_t>(procedureId);
    _procedures[id] = info;
    _nextId = static_cast<ProcedureId>(id + 1);
    return procedureId;
}

const ProcedureTableEntry* ProcedureTable::FindProcedure(ProcedureId procedureId) const
{
    if (procedureId == ProcedureId::Invalid) {
        return nullptr;
    }

    const auto id = static_cast<uint8_t>(procedureId);
    if (id >= _procedures.size()) {
        return nullptr;
    }

    const ProcedureTableEntry* info = &_procedures[id];
    if (!info->IsValid()) {
        return nullptr;
    }

    return info;
}
