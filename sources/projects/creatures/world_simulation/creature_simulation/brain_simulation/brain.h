#pragma once
#include <array>

/// Brain config
/// @{
inline constexpr size_t BrainInputCount = 10;

inline constexpr size_t BrainHiddenNeuronsCount = 10;

inline constexpr size_t BrainLinearOutputCount = 10;
inline constexpr size_t BrainSigmoidOutputCount = 10;
/// @}

inline constexpr size_t BrainTotalOutputCount = BrainLinearOutputCount + BrainSigmoidOutputCount;

struct BrainNeurons {
    std::array<float, BrainInputCount> biasInputToHidden;
    std::array<float, BrainHiddenNeuronsCount * BrainInputCount> weightInputToHidden;
    std::array<float, BrainHiddenNeuronsCount> hiddenQuality;

    std::array<float, BrainTotalOutputCount * BrainHiddenNeuronsCount> weightHiddenToOutput;
    std::array<float, BrainTotalOutputCount> biasHiddenToOutput;
};

struct BrainInput {
    std::array<float, BrainInputCount> values;
};

struct BrainOutput {
    std::array<float, BrainLinearOutputCount> linearValues;
    std::array<float, BrainSigmoidOutputCount> sigmoidValues;
};

void EvaluateBrain(ConstRef<BrainInput> brainInput, ConstRef<BrainNeurons> neurons, Ref<BrainOutput> output);
