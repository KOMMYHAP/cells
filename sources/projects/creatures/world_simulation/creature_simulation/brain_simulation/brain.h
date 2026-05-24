#pragma once
#include <algorithm> //< std::clamp
#include <utility> //< std::max
#include <cmath> //< std::exp
#include <array>

template<class Impl>
struct BrainConfigTraits {
    static consteval size_t GetInputCount() { return Impl::GetInputCount(); }
    static consteval size_t GetHiddenNeuronsCount() { return Impl::GetHiddenNeuronsCount(); }
    static consteval size_t GetLinearOutputCount() { return Impl::GetLinearOutputCount(); }
    static consteval size_t GetSigmoidOutputCount() { return Impl::GetSigmoidOutputCount(); }

    static consteval size_t GetTotalOutputCount() { return GetLinearOutputCount() + GetSigmoidOutputCount(); }
};

template<class Impl>
struct BrainEvaluator {
    using Config = BrainConfigTraits<Impl>;

    struct BrainInput {
        std::array<float, Config::GetInputCount()> values;
    };

    struct BrainNeurons {
        std::array<float, Config::GetInputCount()> biasInputToHidden;
        std::array<float, Config::GetHiddenNeuronsCount() * Config::GetInputCount()> weightInputToHidden;
        std::array<float, Config::GetHiddenNeuronsCount()> hiddenQuality;

        std::array<float, Config::GetTotalOutputCount() * Config::GetHiddenNeuronsCount()> weightHiddenToOutput;
        std::array<float, Config::GetTotalOutputCount()> biasHiddenToOutput;
    };

    struct BrainOutput {
        std::array<float, Config::GetLinearOutputCount()> linearValues;
        std::array<float, Config::GetSigmoidOutputCount()> sigmoidValues;
    };


    void Evaluate(ConstRef<BrainInput> brainInput, ConstRef<BrainNeurons> neurons, Ref<BrainOutput> output) const;
};

#include "brain.hpp"
