#pragma once

inline float ActivationReLU(float x) {
    return std::max(0.0f, x);
}

inline float ActivationLinear(float x) {
    return std::clamp(-1.0f, 1.0f, x);
}

inline float ActivationSigmoid(float x) {
    return 1.0f / (1.0f + std::exp(-x));
}


template<class Impl>
void BrainEvaluator<Impl>::Evaluate(ConstRef<BrainInput> brainInput, ConstRef<BrainNeurons> neurons, Ref<BrainOutput> output) const {
    std::array<float, Config::GetHiddenNeuronsCount()> hiddenLayer{};

    for (size_t hiddenNeuronIndex{0}; hiddenNeuronIndex < Config::GetHiddenNeuronsCount(); ++hiddenNeuronIndex) {
        const float bias = neurons->biasInputToHidden[hiddenNeuronIndex];
        float sum = bias;

        for (size_t inputIndex{0}; inputIndex < Config::GetInputCount(); ++inputIndex) {
            const float input = brainInput->values[inputIndex];
            const float weight = neurons->weightInputToHidden[hiddenNeuronIndex * Config::GetInputCount() + inputIndex];
            sum += input * weight;
        }

        const float quality = neurons->hiddenQuality[hiddenNeuronIndex];
        hiddenLayer[hiddenNeuronIndex] = ActivationReLU(quality * sum);
    }

    for (size_t linearOutputIndex{0}; linearOutputIndex < Config::GetLinearOutputCount(); ++linearOutputIndex) {
        const float biasValue = neurons->biasHiddenToOutput[linearOutputIndex];
        float sumValue = biasValue;
        for (size_t hiddenNeuronIndex{0}; hiddenNeuronIndex < Config::GetHiddenNeuronsCount(); ++hiddenNeuronIndex) {
            const float hiddenValue = hiddenLayer[hiddenNeuronIndex];
            const float hiddenWeightValue = neurons->weightHiddenToOutput[linearOutputIndex * Config::GetHiddenNeuronsCount() + hiddenNeuronIndex];
            sumValue += hiddenValue * hiddenWeightValue;
        }

        output->linearValues[linearOutputIndex] = ActivationLinear(sumValue);
    }

    for (size_t sigmoidOutputIndex{0}; sigmoidOutputIndex < Config::GetSigmoidOutputCount(); ++sigmoidOutputIndex) {
        const float biasValue = neurons->biasHiddenToOutput[Config::GetLinearOutputCount() + sigmoidOutputIndex];
        float sumValue = biasValue;
        for (size_t hiddenNeuronIndex{0}; hiddenNeuronIndex < Config::GetHiddenNeuronsCount(); ++hiddenNeuronIndex) {
            const float hiddenValue = hiddenLayer[hiddenNeuronIndex];
            const size_t weightIndex = (Config::GetLinearOutputCount() + sigmoidOutputIndex) * Config::GetHiddenNeuronsCount() + hiddenNeuronIndex;
            const float hiddenWeightValue = neurons->weightHiddenToOutput[weightIndex];
            sumValue += hiddenValue * hiddenWeightValue;
        }

        output->sigmoidValues[sigmoidOutputIndex] = ActivationSigmoid(sumValue);
    }
}
