#include "ecs_config.h"

enum class SensorType : uint8_t {
    CellEnergy,
    CellRotation,

    SoundArea_0_0,
    SoundArea_0_1,
    SoundArea_0_2,
    SoundArea_1_0,
    SoundArea_1_1,
    SoundArea_1_2,
    SoundArea_2_0,
    SoundArea_2_1,
    SoundArea_2_2,

    TouchArea_0_0,
    TouchArea_0_1,
    TouchArea_0_2,
    TouchArea_1_0,
    TouchArea_1_1,
    TouchArea_1_2,
    TouchArea_2_0,
    TouchArea_2_1,
    TouchArea_2_2,

    SmellArea_0_0,
    SmellArea_0_1,
    SmellArea_0_2,
    SmellArea_1_0,
    SmellArea_1_1,
    SmellArea_1_2,
    SmellArea_2_0,
    SmellArea_2_1,
    SmellArea_2_2,

    PhotoArea_0_0,
    PhotoArea_0_1,
    PhotoArea_0_2,
    PhotoArea_1_0,
    PhotoArea_1_1,
    PhotoArea_1_2,
    PhotoArea_2_0,
    PhotoArea_2_1,
    PhotoArea_2_2,

    ThermoArea_0_0, //< is it hot or cold?
    ThermoArea_0_1,
    ThermoArea_0_2,
    ThermoArea_1_0,
    ThermoArea_1_1,
    ThermoArea_1_2,
    ThermoArea_2_0,
    ThermoArea_2_1,
    ThermoArea_2_2,

    _Count
};
static constexpr uint8_t SensorTypeCount = static_cast<uint8_t>(SensorType::_Count);

enum class GeneType : uint8_t {
    GeneCellMutationRootFactor,
    GeneCellMutationFactorSensorsWeight,
    GeneCellMutationFactorHiddenBias,
    GeneCellMutationFactorHiddenWeight,
    GeneCellMutationFactorActionBias,

    GeneCellEnergySensor,
    GeneCellRotationSensor,

    GeneSoundAreaSensor,
    GeneTouchAreaSensor,
    GeneSmellAreaSensor,
    GenePhotoAreaSensor,
    GeneThermoAreaSensor,

    GeneCanMakeSound,
    GeneCanMakeMove,
    GeneCanMakeTemperature,
    GeneCanMakeSmell,
    GeneCanMakeFlash,
    GeneCanMakeRotation,

    _Count
};
static constexpr uint8_t GeneTypeCount = static_cast<uint8_t>(GeneType::_Count);

enum class ActionType : uint8_t {
    Idle,

    ReadCellEnergy,
    ReadCellRotation,

    ReadAreaSound,
    ReadAreaTouch,
    ReadAreaSmell,
    ReadAreaPhoto,
    ReadAreaThermo,

    Bite,
    Move,
    Rotate,

    MakeSound,
    MakeSmell,
    MakeTemperature,
    MakeFlash,

    _Count
};
static constexpr uint8_t ActionTypeCount = static_cast<uint8_t>(ActionType::_Count);

struct BrainSensorsComponent {
    std::array<float, SensorTypeCount> data;
};

struct BrainSpecializationComponent {
    std::array<float, GeneTypeCount * SensorTypeCount> weightSensorToHidden;
    std::array<float, GeneTypeCount> biasSensorToHidden;
    std::array<float, GeneTypeCount * ActionTypeCount> weightsHiddenToOutput;
    std::array<float, ActionTypeCount> biasHiddenToOutput;
};

struct BrainReactionComponent {
    std::array<float, ActionTypeCount> actions;
};

struct BrainContext {
    ConstRef<BrainSensorsComponent> sensors;
    ConstRef<BrainSpecializationComponent> specialization;
    Ref<BrainReactionComponent> output;
};

struct CellMutationFactorComponent {
    float rootSigma { 0.0f };
};

struct RandomSourceComponent {
    std::random_device rd;
    std::mt19937 generator;
};

// GeneCellMutationFactorSensorsWeight,
// GeneCellMutationFactorHiddenBias,
// GeneCellMutationFactorHiddenWeight,
// GeneCellMutationFactorActionBias,
void MutateRootFactor(CellMutationFactorComponent& mutation, RandomSourceComponent& random)
{
    static constexpr float SigmaMutationSpeed = 0.01f;
    std::normal_distribution<float> distribution { 0.0f, SigmaMutationSpeed };
    const float sigmaMutationFactor = distribution(random.generator);
    mutation.rootSigma = mutation.rootSigma * std::exp(sigmaMutationFactor);
}

static float ActivationReLU(float x)
{
    return std::max(0.0f, x);
}

static float ActivationSigma(float x)
{
    return 1.0f / (1.0f + std::exp(-x));
}

void EvaluateBrain(const BrainContext& context)
{
    std::array<float, GeneTypeCount> hiddenLayer;

    for (const uint8_t geneIndex : std::views::iota(static_cast<uint8_t>(0), GeneTypeCount)) {
        const float bias = context.specialization->biasSensorToHidden[geneIndex];
        float sum = bias;

        for (const uint8_t sensorIndex : std::views::iota(static_cast<uint8_t>(0), SensorTypeCount)) {
            const float sensorValue = context.sensors->data[sensorIndex];
            const float sensorWeight = context.specialization->weightSensorToHidden[geneIndex * GeneTypeCount + sensorIndex];
            sum += sensorValue * sensorWeight;
        }

        hiddenLayer[geneIndex] = ActivationReLU(sum);
    }

    for (const uint8_t actionIndex : std::views::iota(static_cast<uint8_t>(0), ActionTypeCount)) {
        const float bias = context.specialization->biasHiddenToOutput[actionIndex];
        float sum = bias;

        for (const uint8_t geneIndex : std::views::iota(static_cast<uint8_t>(0), GeneTypeCount)) {
            const float hiddenValue = hiddenLayer[geneIndex];
            const float hiddenWeight = context.specialization->weightsHiddenToOutput[actionIndex * ActionTypeCount + geneIndex];
            sum += hiddenValue * hiddenWeight;
        }

        context.output->actions[actionIndex] = ActivationSigma(sum);
    }
}


struct CellGenome {
    std::array<bool, GeneTypeCount> genes;
};

struct WorldPosition {
    uint16_t x{0};
    uint16_t y{0};
};


struct WorldArea {
    EcsEntity d;
};

int main()
{
    return 0;
}