#pragma once

#include "instructions/instructions.h"

namespace processor {

template <class Instruction>
concept ValidInstruction = requires(Instruction instruction) {
    {
        Instruction::argsInCount
    } -> std::convertible_to<uint8_t>;
} && requires(Instruction instruction) {
    {
        Instruction::argsOutCount
    } -> std::convertible_to<uint8_t>;
} && requires(Instruction instruction) {
    (Instruction::argsInCount + Instruction::argsOutCount) < ProcessorInstructionCount;
};

}
