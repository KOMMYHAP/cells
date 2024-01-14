#pragma once

#include "procedure.h"

inline constexpr uint8_t ProcedureInputArgsCountLimit = 4;
inline constexpr uint8_t ProcedureOutputArgsCountLimit = 2;
inline constexpr uint8_t ProcedureTableLimit = 32;

enum class ProcedureId : uint8_t {
    Invalid = std::numeric_limits<uint8_t>::max()
};

struct ProcedureInfo {
    bool IsValid() const { return procedure != nullptr; }

    uint8_t inputArgsCount { 0 };
    uint8_t outputArgsCount { 0 };

    /// Procedure is heavy command, which can be called once per simulation tick,
    /// so we can ignore overhead of virtual call.
    ProcedureBase* procedure { nullptr };
};

class ProcedureTable {
public:
    ProcedureId RegisterProcedure(const ProcedureInfo& info);

    const ProcedureInfo* FindProcedure(ProcedureId id) const;

private:
    std::array<ProcedureInfo, ProcedureTableLimit> _procedures {};
    ProcedureId _nextId { static_cast<ProcedureId>(0) };
};
