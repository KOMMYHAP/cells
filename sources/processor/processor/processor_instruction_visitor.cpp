#include "processor_instruction_visitor.h"
#include <algorithm>
#include <array>

ProcessorInstructionVisitor::ProcessorInstructionVisitor(ProcessorInstruction instruction)
    : _instruction(instruction)
{
}

constexpr int factorial(int n)
{
    return n == 0 ? 1 : n * factorial(n - 1);
}

template <typename T, std::size_t N, typename F>
constexpr auto
transform_array(std::array<T, N> array, F f)
{
    auto array_f = std::array<std::invoke_result_t<F, T>, N> {};
    // This is a constexpr "loop":
    std::transform(array.begin(), array.end(), array_f.begin(), [&f](auto el) { return f(el); });
    return array_f;
}

int foo()
{
    constexpr std::array<int, 4> ints { { 1, 2, 3, 4 } };
    // This can be done at compile time!
    constexpr std::array<int, 4> facts = transform_array(ints, factorial);
    static_assert(facts == std::array<int, 4> { { 1, 2, 6, 24 } }, "");
}