#pragma once

// Please, make sure that ProcedureType is a contiguous sequence without a hole.
// See more in SimulationVirtualMachine::GetProcedureId.
enum class ProcedureType : uint8_t {
    Move,
    Look
};