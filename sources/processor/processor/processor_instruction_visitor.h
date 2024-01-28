#pragma once

#include "instructions/instruction_list.h"
#include "instructions/nope.h"
#include "processor_instruction_concept.h"

namespace processor::details {
struct UnknownInstructionTag { };
template <ProcessorInstruction instruction>
static constexpr auto GetInstructionType()
{
    using namespace processor::instructions;

    if constexpr (instruction == ProcessorInstruction::Nope) {
        return Nope {};
    } else {
        return UnknownInstructionTag {};
    }
    //
    //        switch (instruction) {
    //        case ProcessorInstruction::Nope:
    //            return Wrapper<Nope> {};
    //        case ProcessorInstruction::WriteRegistryValue:
    //            break;
    //        case ProcessorInstruction::WriteRegistryRegistry:
    //            break;
    //        case ProcessorInstruction::CompareRegistryRegistry:
    //            break;
    //        case ProcessorInstruction::CompareRegistryValue:
    //            break;
    //        case ProcessorInstruction::AddRegistryValue:
    //            break;
    //        case ProcessorInstruction::AddRegistryRegistry:
    //            break;
    //        case ProcessorInstruction::SubtractRegistryValue:
    //            break;
    //        case ProcessorInstruction::SubtractRegistryRegistry:
    //            break;
    //        case ProcessorInstruction::Jump:
    //            break;
    //        case ProcessorInstruction::JumpIfEqual:
    //            break;
    //        case ProcessorInstruction::JumpIfNotEqual:
    //            break;
    //        case ProcessorInstruction::JumpIfLess:
    //            break;
    //        case ProcessorInstruction::JumpIfLessEqual:
    //            break;
    //        case ProcessorInstruction::JumpIfGreater:
    //            break;
    //        case ProcessorInstruction::JumpIfGreaterEqual:
    //            break;
    //        case ProcessorInstruction::Call:
    //            break;
    //        case ProcessorInstruction::PushStackRegistry:
    //            break;
    //        case ProcessorInstruction::PushStackValue:
    //            break;
    //        case ProcessorInstruction::PopStackRegistry:
    //            break;
    //        case ProcessorInstruction::LastProcessorInstruction:
    //        default:
    //        }
    //        return Wrapper<UnknownInstruction> {};
}



}

namespace processor {

template <ProcessorInstruction T>
using ProcessorInstructionType = decltype(details::GetInstructionType<T>());


}

class ProcessorInstructionVisitor {
public:
    ProcessorInstructionVisitor(ProcessorInstruction instruction);

    template <class Func>
    void Apply()
    {
    }

    template <class T>
    struct Wrapper {
        using type = T;
    };
    struct UnknownInstruction { };

    static_assert(std::is_same_v<GetInstructionType<ProcessorInstruction::Nope>(), processor::instructions::Nope>, "bla bla");

private:
    ProcessorInstruction _instruction;
};