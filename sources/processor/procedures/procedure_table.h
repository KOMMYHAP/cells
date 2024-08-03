#pragma once

#include "procedure.h"
#include "procedure_id.h"

inline constexpr uint8_t ProcedureInputArgsCountLimit = 2;
inline constexpr uint8_t ProcedureOutputArgsCountLimit = 1;

struct ProcedureTableEntry {
    uint8_t inputArgsCount { 0 };
    uint8_t outputArgsCount { 0 };
    ProcedureBase* procedure { nullptr };
};

class ProcedureTable {
public:
    ProcedureId RegisterProcedure(const ProcedureTableEntry& info);

    /// If entry found, it will be valid
    const ProcedureTableEntry* FindProcedure(ProcedureId procedureId) const;

private:
    std::array<ProcedureTableEntry, ProcedureTableLimit> _procedures {};
    ProcedureId _nextId { static_cast<ProcedureId>(0) };
};
