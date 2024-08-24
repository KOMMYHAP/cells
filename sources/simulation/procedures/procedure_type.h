#pragma once

enum class ProcedureType : uint8_t {
    Move,
    Look,
    Reproduction,
    LastProcedureType
};

inline constexpr uint8_t ProcedureCountLimit = static_cast<uint8_t>(ProcedureType::LastProcedureType);