#pragma once

#include "processor/instructions/instruction_list.h"
#include "processor_control_block.h"

class ProcessorContext;

namespace processor {

template <class Instruction>
concept ValidInstruction = requires(Instruction instruction) {
    {
        Instruction::stackInCount
    } -> std::convertible_to<uint8_t>;
} && requires(Instruction instruction) {
    {
        Instruction::stackOutCount
    } -> std::convertible_to<uint8_t>;
} && requires(Instruction instruction) {
    {
        Instruction::registerInCount
    } -> std::convertible_to<uint8_t>;
} && requires(Instruction instruction) {
    {
        Instruction::registerOutCount
    } -> std::convertible_to<uint8_t>;
} && requires(Instruction instruction) {
    std::max(Instruction::stackInCount, Instruction::stackOutCount) < ProcessorStackSize;
} && requires(Instruction instruction) {
    std::max(Instruction::registerInCount, Instruction::registerOutCount) < ProcessorRegistryCount;
} && requires(Instruction instruction, ProcessorContext context) {
    {
        instruction.Execute(context)
    } -> std::convertible_to<bool>;
};

}
