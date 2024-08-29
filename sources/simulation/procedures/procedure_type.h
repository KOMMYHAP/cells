#pragma once

// todo: remove constants, move to runtime
enum class ProcedureType : uint8_t {
    Move,
    Look,
    SpawnRandomCell,
    LastProcedureType
};

inline constexpr uint8_t ProcedureCountLimit = static_cast<uint8_t>(ProcedureType::LastProcedureType);