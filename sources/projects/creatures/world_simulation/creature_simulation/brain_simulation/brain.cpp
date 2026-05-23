#include "brain.h"

static float ActivationReLU(float x) {
    return std::max(0.0f, x);
}

static float ActivationLinear(float x) {
    return std::clamp(-1.0f, 1.0f, x);
}

static float ActivationSigmoid(float x) {
    return 1.0f / (1.0f + std::exp(-x));
}

void EvaluateBrain(ConstRef<BrainInput> brainInput, ConstRef<BrainNeurons> neurons, Ref<BrainOutput> output) {
    std::array<float, BrainHiddenNeuronsCount> hiddenLayer{};

    for (size_t hiddenNeuronIndex{0}; hiddenNeuronIndex < BrainHiddenNeuronsCount; ++hiddenNeuronIndex) {
        const float bias = neurons->biasInputToHidden[hiddenNeuronIndex];
        float sum = bias;

        for (size_t inputIndex{0}; inputIndex < BrainInputCount; ++inputIndex) {
            const float input = brainInput->values[inputIndex];
            const float weight = neurons->weightInputToHidden[hiddenNeuronIndex * BrainHiddenNeuronsCount + inputIndex];
            sum += input * weight;
        }

        const float quality = neurons->hiddenQuality[hiddenNeuronIndex];
        hiddenLayer[hiddenNeuronIndex] = ActivationReLU(quality * sum);
    }

    for (size_t linearOutputIndex{0}; linearOutputIndex < BrainLinearOutputCount; ++linearOutputIndex) {
        const float biasValue = neurons->biasHiddenToOutput[linearOutputIndex];
        float sumValue = biasValue;
        for (size_t hiddenNeuronIndex{0}; hiddenNeuronIndex < BrainHiddenNeuronsCount; ++hiddenNeuronIndex) {
            const float hiddenValue = hiddenLayer[hiddenNeuronIndex];
            const float hiddenWeightValue = neurons->weightHiddenToOutput[linearOutputIndex * BrainLinearOutputCount + hiddenNeuronIndex];
            sumValue += hiddenValue * hiddenWeightValue;
        }

        output->linearValues[linearOutputIndex] = ActivationLinear(sumValue);
    }

    for (size_t sigmoidOutputIndex{0}; sigmoidOutputIndex < BrainSigmoidOutputCount; ++sigmoidOutputIndex) {
        const float biasValue = neurons->biasHiddenToOutput[BrainLinearOutputCount + sigmoidOutputIndex];
        float sumValue = biasValue;
        for (size_t hiddenNeuronIndex{0}; hiddenNeuronIndex < BrainHiddenNeuronsCount; ++hiddenNeuronIndex) {
            const float hiddenValue = hiddenLayer[hiddenNeuronIndex];
            const float hiddenWeightValue = neurons->weightHiddenToOutput[BrainLinearOutputCount + sigmoidOutputIndex * BrainSigmoidOutputCount + hiddenNeuronIndex];
            sumValue += hiddenValue * hiddenWeightValue;
        }

        output->sigmoidValues[sigmoidOutputIndex] = ActivationSigmoid(sumValue);
    }
}
