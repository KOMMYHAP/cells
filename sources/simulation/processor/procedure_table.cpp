#include "procedure_table.h"

ProcedureId ProcedureTable::RegisterProcedure(const ProcedureInfo& info)
{
    ProcedureId id = _nextId;
    if (id == ProcedureId::Invalid) {
        return ProcedureId::Invalid;
    }

    if (info.inputArgsCount >= ProcedureInputArgsCountLimit) {
        return ProcedureId::Invalid;
    }
    if (info.outputArgsCount >= ProcedureOutputArgsCountLimit) {
        return ProcedureId::Invalid;
    }

    const auto _id = static_cast<uint8_t>(id);
    _procedures[_id] = info;
    _nextId = static_cast<ProcedureId>(_id + 1);
    return id;
}

const ProcedureInfo* ProcedureTable::FindProcedure(ProcedureId id) const
{
    if (id == ProcedureId::Invalid) {
        return nullptr;
    }

    const auto _id = static_cast<uint8_t>(id);
    if (_procedures.size() >= _id) {
        return nullptr;
    }

    const ProcedureInfo* info = &_procedures[_id];
    if (!info->IsValid()) {
        return nullptr;
    }

    return info;
}
