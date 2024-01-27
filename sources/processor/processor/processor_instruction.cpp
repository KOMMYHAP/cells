#include "processor_instruction.h"

#include "instructions/instruction_nope.h"

namespace processor::instructions {
static_assert(ValidInstruction<Nope>);
}
