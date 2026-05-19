#include "ecs_config.h"
#include "components/generated/auto_cell_position.h"
#include "SDL3/SDL.h"
#include "widgets/world/world_rasterization_target.h"

enum class Sensors : uint8_t {
    CreatureEnergy,
    CreatureRotation,
    CreatureFatigue,

    // SoundArea_0_0,
    // SoundArea_0_1,
    // SoundArea_0_2,
    // SoundArea_1_0,
    // SoundArea_1_1,
    // SoundArea_1_2,
    // SoundArea_2_0,
    // SoundArea_2_1,
    // SoundArea_2_2,

    TouchArea_0_0,
    TouchArea_0_1,
    TouchArea_0_2,
    TouchArea_1_0,
    TouchArea_1_1,
    TouchArea_1_2,
    TouchArea_2_0,
    TouchArea_2_1,
    TouchArea_2_2,

    // SmellArea_0_0,
    // SmellArea_0_1,
    // SmellArea_0_2,
    // SmellArea_1_0,
    // SmellArea_1_1,
    // SmellArea_1_2,
    // SmellArea_2_0,
    // SmellArea_2_1,
    // SmellArea_2_2,
    //
    // PhotoArea_0_0,
    // PhotoArea_0_1,
    // PhotoArea_0_2,
    // PhotoArea_1_0,
    // PhotoArea_1_1,
    // PhotoArea_1_2,
    // PhotoArea_2_0,
    // PhotoArea_2_1,
    // PhotoArea_2_2,
    //
    // ThermoArea_0_0, //< is it hot or cold?
    // ThermoArea_0_1,
    // ThermoArea_0_2,
    // ThermoArea_1_0,
    // ThermoArea_1_1,
    // ThermoArea_1_2,
    // ThermoArea_2_0,
    // ThermoArea_2_1,
    // ThermoArea_2_2,

    InternalCount
};

static constexpr uint8_t SensorsCount = static_cast<uint8_t>(Sensors::InternalCount);

struct SensorDescription {
    std::string_view name;
    int32_t minValue{0}; //< included
    int32_t maxValue{0}; //< excluded
};

enum class SensorCalculationResult {
    MinValueReached,
    ValueInRange,
    MaxValueReached,
};

SensorCalculationResult UpdateSensorValue(const SensorDescription &desc, uint16_t &value, int32_t diff) {
    const int32_t newValue = static_cast<int32_t>(value) + diff;
    if (newValue >= desc.maxValue) {
        value = static_cast<uint16_t>(desc.maxValue);
        return SensorCalculationResult::MaxValueReached;
    }
    if (newValue <= desc.minValue) {
        value = static_cast<uint16_t>(desc.minValue);
        return SensorCalculationResult::MinValueReached;
    }
    value = static_cast<uint16_t>(newValue);
    return SensorCalculationResult::ValueInRange;
}

uint16_t ConvertFromSensorValue(const float value, const SensorDescription &desc) {
    const float rawValue = std::lerp(static_cast<float>(desc.minValue), static_cast<float>(desc.maxValue) - 1.0f, value);
    return static_cast<uint16_t>(std::roundf(rawValue));
}

template<class T>
    requires std::is_enum_v<T>
T ConvertFromSensorValue(const float value, const T minValue = T{}, const T maxValue = T::InternalCount) {
    const float rawValue = std::lerp(static_cast<float>(minValue), static_cast<float>(maxValue) - 1.0f, value);
    return static_cast<T>(static_cast<std::underlying_type_t<T>>(std::roundf(rawValue)));
}

float ConvertToSensorValue(const uint16_t value, const SensorDescription &desc) {
    const float rawValue = (static_cast<float>(value) - static_cast<float>(desc.minValue)) / static_cast<float>(desc.maxValue - desc.minValue);
    return std::clamp(rawValue, 0.0f, 1.0f);
}

template<class T>
    requires std::is_enum_v<T>
float ConvertToSensorValue(const T value, const T minValue = T{}, const T maxValue = T::InternalCount) {
    const float rawValue = (static_cast<float>(value) - static_cast<float>(minValue)) / static_cast<float>(std::to_underlying(maxValue) - std::to_underlying(minValue));
    return std::clamp(rawValue, 0.0f, 1.0f);
}


enum class Genes : uint8_t {
    // GeneCreatureMutationFactorSensorsWeight,
    // GeneCreatureMutationFactorHiddenBias,
    // GeneCreatureMutationFactorHiddenWeight,
    // GeneCreatureMutationFactorActionBias,

    GeneCreatureEnergySensor,
    GeneCreatureRotationSensor,
    GeneCreatureFatigueSensor,

    // GeneSoundAreaSensor,
    GeneTouchAreaSensor,
    // GeneSmellAreaSensor,
    // GenePhotoAreaSensor,
    // GeneThermoAreaSensor,

    // GeneCanMakeSound,
    GeneCanMove,
    GeneCanBite,
    // GeneCanMakeTemperature,
    // GeneCanMakeSmell,
    // GeneCanMakeFlash,
    GeneCanRotate,
    GeneCanMakeChild,

    GeneBrainRestSpeed,
    GeneBitePower,

    InternalCount
};

static constexpr uint8_t GenesCount = static_cast<uint8_t>(Genes::InternalCount);


enum class GeneTypes {
    Ability, //< has or not?
    Property, //< how much?
    InternalCount
};

static constexpr uint8_t GeneTypesCount = static_cast<uint8_t>(GeneTypes::InternalCount);

struct GeneDescription {
    std::string_view name;
    GeneTypes type{GeneTypes::InternalCount};
    uint16_t minValue{0};
    uint16_t maxValue{0};
    float mutationCenter{0.0f};
    float mutationSigma{0.0f};
};

enum class AvailabilityResult {
    Yes,
    No,
    InternalCount
};

static constexpr uint8_t AvailabilityResultCount = static_cast<uint8_t>(AvailabilityResult::InternalCount);

enum class Actions : uint8_t {
    Idle,

    ReadCreatureEnergy,
    ReadCreatureRotation,

    // ReadAreaSound,
    ReadAreaTouch,
    // ReadAreaSmell,
    // ReadAreaPhoto,
    // ReadAreaThermo,

    Bite,
    Move,
    Rotate,

    // MakeSound,
    // MakeSmell,
    // MakeTemperature,
    // MakeFlash,
    MakeChild,

    InternalCount
};

struct ActionDescription {
    std::string_view name;
    int32_t energyCost{0};
    int32_t brainTickCost{0};
};

static constexpr uint8_t ActionTypeCount = static_cast<uint8_t>(Actions::InternalCount);

struct WorldDescription {
    std::mt19937 randomGenerator;

    std::array<ActionDescription, ActionTypeCount> actionRules{};
    std::array<GeneDescription, GenesCount> geneRules{};
    std::array<SensorDescription, SensorsCount> sensorRules{};
    std::array<bool, ActionTypeCount * GenesCount> actionsRequiredGene{}; //< which genes are required for this action?
    std::array<bool, ActionTypeCount * GenesCount> actionsForbiddenGene{}; //< which genes are forbidden for this action?

    float neuronValueRangeSize{0.0f}; //< range = [-x / 2; x / 2]

    int32_t brainRestPerTickMin{0};
    int32_t creatureEnergyMax{0};
    int32_t creatureFatigueMax{0};
    int32_t consumeEnergyPerBiteMax{0};
};

struct BrainSensorsComponent {
    std::array<float, SensorsCount> data;
};

struct BrainGenomeComponent {
    std::array<float, GenesCount> quality;
};

struct BrainSpecializationComponent {
    std::array<float, GenesCount * SensorsCount> weightSensorToHidden;
    std::array<float, GenesCount * GenesCount> weightQualityToHidden;
    std::array<float, GenesCount> biasSensorToHidden;
    std::array<float, GenesCount * ActionTypeCount> weightsHiddenToOutput;
    std::array<float, ActionTypeCount> biasHiddenToOutput;
};

struct BrainReactionComponent {
    std::array<float, ActionTypeCount> actionsValue; //< [0; 1]
};

struct BrainContext {
    ConstRef<BrainSensorsComponent> sensors;
    ConstRef<BrainGenomeComponent> genome;
    ConstRef<BrainSpecializationComponent> specialization;
    Ref<BrainReactionComponent> output;
};

// struct CreatureMutationFactorComponent {
//     float rootSigma{0.0f};
// };

// struct RandomSourceComponent {
//     Ref<std::mt19937> generator;
// };
//
// void MutateRootFactor(CreatureMutationFactorComponent &mutation, RandomSourceComponent &random) {
//     static constexpr float SigmaMutationSpeed = 0.01f;
//     std::normal_distribution distribution{0.0f, SigmaMutationSpeed};
//     const float sigmaMutationFactor = distribution(*random.generator);
//     mutation.rootSigma = mutation.rootSigma * std::exp(sigmaMutationFactor);
// }

static float ActivationReLU(float x) {
    return std::max(0.0f, x);
}

static float ActivationSigma(float x) {
    return 1.0f / (1.0f + std::exp(-x));
}

void EvaluateBrain(const BrainContext &context) {
    std::array<float, GenesCount> hiddenLayer{};

    for (const uint8_t geneIndex: std::views::iota(static_cast<uint8_t>(0), GenesCount)) {
        const float bias = context.specialization->biasSensorToHidden[geneIndex];
        float sum = bias;

        for (const uint8_t sensorIndex: std::views::iota(static_cast<uint8_t>(0), SensorsCount)) {
            const float sensorValue = context.sensors->data[sensorIndex];
            const float sensorWeight = context.specialization->weightSensorToHidden[geneIndex * GenesCount + sensorIndex];
            sum += sensorValue * sensorWeight;
        }
        for (const uint8_t qualityIndex: std::views::iota(static_cast<uint8_t>(0), GenesCount)) {
            const float quality = context.genome->quality[qualityIndex];
            const float sensorWeight = context.specialization->weightQualityToHidden[geneIndex * GenesCount + qualityIndex];
            sum += quality * sensorWeight;
        }

        const float geneQuality = context.genome->quality[geneIndex];
        hiddenLayer[geneIndex] = ActivationReLU(geneQuality * sum);
    }

    for (const uint8_t actionIndex: std::views::iota(static_cast<uint8_t>(0), ActionTypeCount)) {
        const float biasValue = context.specialization->biasHiddenToOutput[actionIndex];
        float sumValue = biasValue;

        for (const uint8_t geneIndex: std::views::iota(static_cast<uint8_t>(0), GenesCount)) {
            const float hiddenValue = hiddenLayer[geneIndex];
            const float hiddenWeightValue = context.specialization->weightsHiddenToOutput[actionIndex * ActionTypeCount + geneIndex];
            sumValue += hiddenValue * hiddenWeightValue;
        }

        context.output->actionsValue[actionIndex] = ActivationSigma(sumValue);
    }
}


struct CreatureGenomeComponent {
    std::array<bool, GenesCount> genes;
};

struct WorldPosition {
    int16_t x{0};
    int16_t y{0};
};

enum class WorldAreaIndex : uint8_t {
    Position_0_0,
    Position_0_1,
    Position_0_2,
    Position_1_0,
    Position_1_1,
    Position_1_2,
    Position_2_0,
    Position_2_1,
    Position_2_2,
    InternalCount
};

static constexpr int32_t WorldAreaSize = static_cast<int32_t>(WorldAreaIndex::InternalCount);

struct WorldAreaLocatorComponent {
    std::array<EcsEntity, WorldAreaSize> entities;
};


enum class WorldDirection : uint8_t {
    Up,
    Down,
    Left,
    Right,
    InternalCount
};

static constexpr uint8_t WorldDirectionCount = static_cast<uint8_t>(WorldDirection::InternalCount);

struct CreaturePositionComponent {
    EcsEntity value;
};

struct WorldPositionComponent {
    WorldPosition position;
};

enum class TouchResult : uint8_t {
    Nothing,
    // Fluid,
    // Sticky,
    Elastic,
    Solid,
    InternalCount
};

struct WorldObstacleTag {
};

static constexpr int32_t TouchResultSize = static_cast<int32_t>(TouchResult::InternalCount);

struct CreatureBrainIsOverloadedTag {
};

struct CreatureBrainReactionStateComponent {
    uint16_t brainTicksConsumed{0};
};

struct CreatureActionIdleTag {
};

struct CreatureStateEnergyComponent {
    uint16_t value{0};
};

struct CreatureOutOfEnergyTag {
};

struct CreatureStateFatigueComponent {
    uint16_t value{0};
};

struct CreatureStateRotationComponent {
    WorldDirection value{WorldDirection::InternalCount};
};

struct CreatureActionRotateComponent {
    WorldDirection value{WorldDirection::InternalCount};
};

struct CreatureActionMoveComponent {
    WorldDirection value{WorldDirection::InternalCount};
};

struct CreatureSensorTouchComponent {
    std::array<TouchResult, WorldAreaSize> touches;
};

struct CreatureActionReadTouchAreaTag {
};

struct CreatureSensorEnergyComponent {
    uint16_t value{0};
};

struct CreatureActionReadEnergyTag {
};

struct CreatureSensorRotationComponent {
    WorldDirection value{WorldDirection::InternalCount};
};

struct CreatureActionReadRotationTag {
};

struct WorldCreatureComponent {
    EcsEntity creature;
};

struct CreatureActionBiteComponent {
    float amplitude{0.0f};
};


bool TryConsumeEnergy(EcsWorld &world, const WorldDescription &worldDesc, EcsEntity creature, CreatureStateEnergyComponent &energy, Actions action, float amplitude = 1.0f) {
    const SensorDescription &desc = worldDesc.sensorRules[static_cast<uint8_t>(Sensors::CreatureEnergy)];
    const ActionDescription &actionDesc = worldDesc.actionRules[static_cast<uint8_t>(action)];
    const int32_t energyCost = std::max(1, static_cast<int32_t>(std::round(static_cast<float>(actionDesc.energyCost) * amplitude)));
    const SensorCalculationResult r = UpdateSensorValue(desc, energy.value, -energyCost);
    if (r == SensorCalculationResult::MinValueReached) {
        world.emplace<CreatureOutOfEnergyTag>(creature);
        return false;
    }
    return true;
}


void InitCreatureGenome(WorldDescription &worldRules, EcsWorld &world, EcsEntity creature, EcsEntity position) {
    const int32_t initialEnergy = std::uniform_int_distribution{0, worldRules.creatureEnergyMax - 1}(worldRules.randomGenerator);
    const int32_t initialFatigue = std::uniform_int_distribution{0, worldRules.creatureFatigueMax - 1}(worldRules.randomGenerator);
    const int32_t initialRotation = std::uniform_int_distribution{0, WorldDirectionCount - 1}(worldRules.randomGenerator);

    world.emplace<CreatureStateEnergyComponent>(creature, static_cast<uint16_t>(initialEnergy));
    world.emplace<CreatureStateFatigueComponent>(creature, static_cast<uint16_t>(initialFatigue));
    world.emplace<CreatureStateRotationComponent>(creature, static_cast<WorldDirection>(initialRotation));
    world.emplace<CreatureBrainReactionStateComponent>(creature, uint16_t{0});
    world.emplace<CreaturePositionComponent>(creature, position);

    world.emplace<CreatureSensorEnergyComponent>(creature, uint16_t{0});
    world.emplace<CreatureSensorRotationComponent>(creature, WorldDirection::Down);
    auto &[touches] = world.emplace<CreatureSensorTouchComponent>(creature);
    touches.fill(TouchResult::Nothing);

    auto &[actions] = world.emplace<BrainReactionComponent>(creature);
    actions.fill(0.0f);

    auto &[sensors] = world.emplace<BrainSensorsComponent>(creature);
    sensors.fill(0.0f);

    auto FillBrain = [&]<size_t S>(std::array<float, S> &neurons) {
        for (float &neuron: neurons) {
            neuron = std::uniform_real_distribution{-worldRules.neuronValueRangeSize, worldRules.neuronValueRangeSize}(worldRules.randomGenerator);
        }
    };
    auto &brain = world.emplace<BrainSpecializationComponent>(creature);
    FillBrain(brain.biasHiddenToOutput);
    FillBrain(brain.biasSensorToHidden);
    FillBrain(brain.weightQualityToHidden);
    FillBrain(brain.weightSensorToHidden);
    FillBrain(brain.weightsHiddenToOutput);

    auto &[genomeQuality] = world.emplace<BrainGenomeComponent>(creature);
    for (float &quality: genomeQuality) {
        quality = std::uniform_real_distribution{0.0f, 1.0f}(worldRules.randomGenerator);
    }
    auto &[genes] = world.emplace<CreatureGenomeComponent>(creature);
    ASSERT(genes.size() == genomeQuality.size(), "Sanity check: genome size matches quality size");
    for (const size_t geneIndex: std::views::iota(size_t{0}, genes.size())) {
        const bool isAbility = worldRules.geneRules[geneIndex].type == GeneTypes::Ability;
        const bool enabled = isAbility ? static_cast<bool>(std::round(genomeQuality[geneIndex])) : true;
        genes[geneIndex] = enabled;
    }
}


bool IsActionAllowedForGenome(const WorldDescription &worldRules, const CreatureGenomeComponent &genome, Actions action) {
    for (uint8_t geneIndex = 0; geneIndex < GenesCount; ++geneIndex) {
        const bool geneRequired = worldRules.actionsRequiredGene[static_cast<uint8_t>(action) * ActionTypeCount + geneIndex];
        if (geneRequired && !genome.genes[geneIndex]) {
            return false;
        }
    }

    for (uint8_t geneIndex = 0; geneIndex < GenesCount; ++geneIndex) {
        const bool geneForbidden = worldRules.actionsForbiddenGene[static_cast<uint8_t>(action) * ActionTypeCount + geneIndex];
        if (geneForbidden && !genome.genes[geneIndex]) {
            return false;
        }
    }

    return true;
}

void ProcessWorldUpdate(EcsWorld &world, const WorldDescription &worldRules) {
    world.view<BrainSensorsComponent, const CreatureSensorEnergyComponent, const CreatureSensorRotationComponent, const CreatureSensorTouchComponent>().each([&](BrainSensorsComponent &sensors, const CreatureSensorEnergyComponent energySensor, const CreatureSensorRotationComponent rotationSensor, const CreatureSensorTouchComponent &touchSensor) {
        sensors.data[static_cast<uint8_t>(Sensors::CreatureEnergy)] = ConvertToSensorValue(energySensor.value, worldRules.sensorRules[static_cast<uint8_t>(Sensors::CreatureEnergy)]);
        sensors.data[static_cast<uint8_t>(Sensors::CreatureRotation)] = ConvertToSensorValue(rotationSensor.value);
        sensors.data[static_cast<uint8_t>(Sensors::CreatureFatigue)] = ConvertToSensorValue(energySensor.value, worldRules.sensorRules[static_cast<uint8_t>(Sensors::CreatureFatigue)]);

        for (const int32_t touchIndex: std::views::iota(0, WorldAreaSize)) {
            const TouchResult touch = touchSensor.touches[touchIndex];
            sensors.data[static_cast<int32_t>(Sensors::TouchArea_0_0) + touchIndex] = ConvertToSensorValue(touch);
        }
    });

    world.view<const BrainSensorsComponent, const BrainGenomeComponent, const BrainSpecializationComponent, BrainReactionComponent>().each([&](const BrainSensorsComponent &sensors, const BrainGenomeComponent &genome, const BrainSpecializationComponent &specialization, BrainReactionComponent &output) {
        const BrainContext context{ConstRef{&sensors}, ConstRef{&genome}, ConstRef{&specialization}, Ref{&output}};
        EvaluateBrain(context);
    });

    auto BrainDispatchAction = [&](EcsEntity creature, const Actions action, float value) {
        switch (action) {
            case Actions::Idle:
                world.emplace<CreatureActionIdleTag>(creature);
                break;
            case Actions::ReadCreatureEnergy:
                world.emplace<CreatureActionReadEnergyTag>(creature);
                break;
            case Actions::ReadCreatureRotation:
                world.emplace<CreatureActionReadRotationTag>(creature);
                break;
            case Actions::ReadAreaTouch:
                world.emplace<CreatureActionReadTouchAreaTag>(creature);
                break;
            case Actions::Bite:
                world.emplace<CreatureActionBiteComponent>(creature, value);
                break;
            case Actions::Move: {
                const auto direction = ConvertFromSensorValue<WorldDirection>(value);
                world.emplace<CreatureActionMoveComponent>(creature, direction);
            }
            break;
            case Actions::Rotate: {
                const auto direction = ConvertFromSensorValue<WorldDirection>(value);
                world.emplace<CreatureActionRotateComponent>(creature, direction);
            }
            break;
            case Actions::MakeChild:
                break;
            case Actions::InternalCount:
            default:
                ASSERT_FAIL("Sanity check: invalid action type");
                break;
        }
    };

    world.view<const BrainReactionComponent, const CreatureGenomeComponent, CreatureBrainReactionStateComponent>(entt::exclude_t<CreatureBrainIsOverloadedTag>{}).each([&](EcsEntity creature, const BrainReactionComponent &reaction, const CreatureGenomeComponent &genome, CreatureBrainReactionStateComponent &brainState) {
        int32_t brainTickRequested = 0;
        for (uint8_t actionIndex = 0; actionIndex < ActionTypeCount; ++actionIndex) {
            if (!IsActionAllowedForGenome(worldRules, genome, static_cast<Actions>(actionIndex))) {
                continue;
            }

            const float actionValue = reaction.actionsValue[actionIndex];
            BrainDispatchAction(creature, static_cast<Actions>(actionIndex), actionValue);
            brainTickRequested += worldRules.actionRules[actionIndex].brainTickCost;
        }

        brainState.brainTicksConsumed = static_cast<uint16_t>(brainTickRequested);
    });

    world.view<const CreatureBrainReactionStateComponent, CreatureStateFatigueComponent>(entt::exclude_t<CreatureBrainIsOverloadedTag>{}).each([&](EcsEntity creature, const CreatureBrainReactionStateComponent &brainState, CreatureStateFatigueComponent &fatigue) {
        const SensorDescription &desc = worldRules.sensorRules[static_cast<uint8_t>(Sensors::CreatureFatigue)];
        const SensorCalculationResult r = UpdateSensorValue(desc, fatigue.value, brainState.brainTicksConsumed);
        if (r == SensorCalculationResult::MaxValueReached) {
            world.emplace<CreatureBrainIsOverloadedTag>(creature);
        }
    });

    {
        auto overloadedBrains = world.view<const CreatureBrainIsOverloadedTag, const CreaturePositionComponent>();
        world.erase<CreatureBrainIsOverloadedTag>(overloadedBrains.begin(), overloadedBrains.end());
        // overloadedBrains.each([&](const CreaturePositionComponent position) {
        //     // cleanup reference from world to creature
        //     world.erase<WorldCreatureComponent>(position.value);
        // });
        // world.destroy(overloadedBrains.begin(), overloadedBrains.end());
    }

    world.view<CreatureStateFatigueComponent>().each([&](CreatureStateFatigueComponent &fatigue) {
        const SensorDescription &desc = worldRules.sensorRules[static_cast<uint8_t>(Sensors::CreatureFatigue)];
        UpdateSensorValue(desc, fatigue.value, -worldRules.brainRestPerTickMin);
    });

    {
        const auto view = world.view<const CreatureActionIdleTag>();
        world.erase<CreatureActionIdleTag>(view.begin(), view.end());
    }

    world.view<const CreatureActionRotateComponent, CreatureStateRotationComponent, CreatureStateEnergyComponent>(entt::exclude_t<CreatureOutOfEnergyTag>{}).each([&](EcsEntity creature, const CreatureActionRotateComponent &rotate, CreatureStateRotationComponent &rotation, CreatureStateEnergyComponent &energy) {
        const SensorDescription &desc = worldRules.sensorRules[static_cast<uint8_t>(Sensors::CreatureEnergy)];
        const ActionDescription &actionDesc = worldRules.actionRules[static_cast<uint8_t>(Actions::Rotate)];
        const SensorCalculationResult r = UpdateSensorValue(desc, energy.value, -actionDesc.energyCost);
        if (r == SensorCalculationResult::MinValueReached) {
            world.emplace<CreatureOutOfEnergyTag>(creature);
        }

        world.erase<CreatureActionRotateComponent>(creature);
        rotation.value = rotate.value;
    });

    world.view<const CreatureActionMoveComponent, CreaturePositionComponent, CreatureStateEnergyComponent>(entt::exclude_t<CreatureOutOfEnergyTag>{}).each([&](EcsEntity creature, const CreatureActionMoveComponent &move, CreaturePositionComponent &position, CreatureStateEnergyComponent &energy) {
        world.erase<CreatureActionMoveComponent>(creature);
        if (!TryConsumeEnergy(world, worldRules, creature, energy, Actions::Move)) {
            return;
        }

        WorldAreaIndex areaIndex = WorldAreaIndex::InternalCount;
        switch (move.value) {
            case WorldDirection::Up:
                areaIndex = WorldAreaIndex::Position_0_1;
                break;
            case WorldDirection::Down:
                areaIndex = WorldAreaIndex::Position_2_1;
                break;
            case WorldDirection::Left:
                areaIndex = WorldAreaIndex::Position_1_0;
                break;
            case WorldDirection::Right:
                areaIndex = WorldAreaIndex::Position_1_2;
                break;
            case WorldDirection::InternalCount:
                break;
        }

        if (areaIndex == WorldAreaIndex::InternalCount) [[unlikely]] {
            ASSERT_FAIL("Sanity check: unknown move direction!");
            return;
        }

        const auto &[location] = world.get<const WorldAreaLocatorComponent>(position.value);
        const EcsEntity newPosition = location[static_cast<uint8_t>(areaIndex)];
        if (!world.valid(newPosition)) {
            return;
        }
        if (world.any_of<WorldCreatureComponent, WorldObstacleTag>(newPosition)) {
            return;
        }

        const EcsEntity oldPosition = position.value;
        world.erase<WorldCreatureComponent>(oldPosition);
        world.emplace<WorldCreatureComponent>(newPosition, creature);
        position.value = newPosition;
    });

    world.view<const CreatureActionReadTouchAreaTag, const CreaturePositionComponent, CreatureSensorTouchComponent, CreatureStateEnergyComponent>(entt::exclude_t<CreatureOutOfEnergyTag>{}).each([&](EcsEntity creature, const CreaturePositionComponent &creaturePosition, CreatureSensorTouchComponent &touchSensor, CreatureStateEnergyComponent &energy) {
        world.erase<CreatureActionReadTouchAreaTag>(creature);
        if (!TryConsumeEnergy(world, worldRules, creature, energy, Actions::ReadAreaTouch)) {
            return;
        }
        const auto &[entities] = world.get<const WorldAreaLocatorComponent>(creaturePosition.value);
        for (uint8_t i = 0; i < WorldAreaSize; ++i) {
            if (const EcsEntity areaPosition = entities[i]; world.any_of<WorldObstacleTag>(areaPosition)) {
                touchSensor.touches[i] = TouchResult::Solid;
            } else if (world.any_of<WorldCreatureComponent>(areaPosition)) {
                touchSensor.touches[i] = TouchResult::Elastic;
            } else {
                touchSensor.touches[i] = TouchResult::Nothing;
            }
        }
    });

    world.view<const CreatureActionReadEnergyTag, CreatureStateEnergyComponent, CreatureSensorEnergyComponent>(entt::exclude_t<CreatureOutOfEnergyTag>{}).each([&](EcsEntity creature, CreatureStateEnergyComponent &energy, CreatureSensorEnergyComponent &energySensor) {
        world.erase<CreatureActionReadEnergyTag>(creature);
        if (!TryConsumeEnergy(world, worldRules, creature, energy, Actions::ReadCreatureEnergy)) {
            return;
        }
        energySensor.value = energy.value;
    });

    world.view<const CreatureActionReadRotationTag, const CreatureStateRotationComponent, CreatureSensorRotationComponent, CreatureStateEnergyComponent>(entt::exclude_t<CreatureOutOfEnergyTag>{}).each([&](EcsEntity creature, const CreatureStateRotationComponent &rotation, CreatureSensorRotationComponent &rotationSensor, CreatureStateEnergyComponent &energy) {
        world.erase<CreatureActionReadRotationTag>(creature);
        if (!TryConsumeEnergy(world, worldRules, creature, energy, Actions::ReadCreatureRotation)) {
            return;
        }
        rotationSensor.value = rotation.value;
    });


    world.view<const CreatureActionBiteComponent, CreatureStateEnergyComponent, const CreatureStateRotationComponent, const CreaturePositionComponent>(entt::exclude_t<CreatureOutOfEnergyTag>{}).each([&](EcsEntity creature, const CreatureActionBiteComponent bite, CreatureStateEnergyComponent &energy, const CreatureStateRotationComponent rotation, const CreaturePositionComponent position) {
        world.erase<CreatureActionBiteComponent>(creature);
        if (!TryConsumeEnergy(world, worldRules, creature, energy, Actions::Bite, bite.amplitude)) {
            return;
        }

        const auto &[entities] = world.get<WorldAreaLocatorComponent>(position.value);
        if (rotation.value == WorldDirection::InternalCount) {
            return;
        }

        const EcsEntity targetPosition = entities[static_cast<uint8_t>(rotation.value)];
        const WorldCreatureComponent *target = world.try_get<const WorldCreatureComponent>(targetPosition);
        if (!target) {
            return;
        }

        CreatureStateEnergyComponent &targetEnergy = world.get<CreatureStateEnergyComponent>(target->creature);
        const SensorDescription &desc = worldRules.sensorRules[static_cast<uint8_t>(Sensors::CreatureEnergy)];
        const uint16_t oldValue = targetEnergy.value;
        const SensorCalculationResult r = UpdateSensorValue(desc, targetEnergy.value, -worldRules.consumeEnergyPerBiteMax);
        const uint16_t newValue = targetEnergy.value;
        const int32_t consumedEnergy = oldValue - newValue;
        UpdateSensorValue(desc, energy.value, consumedEnergy);
        if (r == SensorCalculationResult::MinValueReached) {
            world.emplace_or_replace<CreatureOutOfEnergyTag>(target->creature);
        }
    });

    {
        auto creaturesToDestroy = world.view<const CreatureOutOfEnergyTag, const CreaturePositionComponent>();
        creaturesToDestroy.each([&](const CreaturePositionComponent position) {
            // cleanup reference from world to creature
            world.erase<WorldCreatureComponent>(position.value);
        });
        world.destroy(creaturesToDestroy.begin(), creaturesToDestroy.end());
    }
}

int main() {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        ASSERT_FAIL("SDL_Init failed");
        return -1;
    }

    static constexpr uint32_t WindowFlags = SDL_WINDOW_HIGH_PIXEL_DENSITY;
    SDL_Window *window = SDL_CreateWindow("Cells", 1200, 800, WindowFlags);
    if (window == nullptr) {
        ASSERT_FAIL("SDL_CreateWindow failed");
        return -1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, nullptr);
    if (renderer == nullptr) {
        ASSERT_FAIL("SDL_CreateRenderer failed");
        return -1;
    }

    SDL_Texture *renderTargetTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, 1200, 800);
    if (!renderTargetTexture) {
        ASSERT_FAIL("SDL_CreateTexture failed");
        return -1;
    }

    WorldRasterizationTarget rasterizationTarget{*renderTargetTexture, SDL_Color{200, 200, 200, SDL_ALPHA_OPAQUE}, 4};

    EcsWorld world{};

    WorldDescription worldRules;
    static constexpr int32_t BrainInputCount = GenesCount + SensorsCount;
    static constexpr float SigmoidMeaningfulRangeSize = 10.0f; //< [-5; 5]
    worldRules.neuronValueRangeSize = static_cast<float>(BrainInputCount) / SigmoidMeaningfulRangeSize;

    worldRules.creatureEnergyMax = 100000;
    worldRules.brainRestPerTickMin = 5;
    worldRules.creatureFatigueMax = 200;
    worldRules.actionsRequiredGene.fill(false);
    worldRules.actionsForbiddenGene.fill(false);

    auto MakeGeneAbility = [&](Genes gene, std::string_view name, float mutationSigma) {
        worldRules.geneRules[static_cast<uint8_t>(gene)] = {name, GeneTypes::Ability, 0, AvailabilityResultCount, 0.0f, mutationSigma};
    };

    worldRules.actionRules[static_cast<uint8_t>(Actions::Idle)] = {"Idle", 1, 0};
    worldRules.actionRules[static_cast<uint8_t>(Actions::ReadCreatureEnergy)] = {"Read Creature Energy", 5, 0};
    worldRules.actionRules[static_cast<uint8_t>(Actions::ReadCreatureRotation)] = {"Read Creature Rotation", 5, 0};
    worldRules.actionRules[static_cast<uint8_t>(Actions::ReadAreaTouch)] = {"Read Area Touch", 15, 10};
    worldRules.actionRules[static_cast<uint8_t>(Actions::Bite)] = {"Bite", 50, 5};
    worldRules.actionRules[static_cast<uint8_t>(Actions::Move)] = {"Move", 15, 5};
    worldRules.actionRules[static_cast<uint8_t>(Actions::Rotate)] = {"Rotate", 10, 5};
    worldRules.actionRules[static_cast<uint8_t>(Actions::MakeChild)] = {"Make Child", 200, 15};

    MakeGeneAbility(Genes::GeneCreatureEnergySensor, "Energy Sensor", 0.01f);
    MakeGeneAbility(Genes::GeneCreatureRotationSensor, "Rotation Sensor", 0.01f);
    MakeGeneAbility(Genes::GeneCreatureFatigueSensor, "Fatigue Sensor", 0.01f);
    MakeGeneAbility(Genes::GeneTouchAreaSensor, "Touch Area Sensor", 0.01f);
    MakeGeneAbility(Genes::GeneCanMove, "Can Move", 0.01f);
    MakeGeneAbility(Genes::GeneCanBite, "Can Bite", 0.01f);
    MakeGeneAbility(Genes::GeneCanRotate, "Can Rotate", 0.01f);
    MakeGeneAbility(Genes::GeneCanMakeChild, "Can Make Child", 0.01f);

    worldRules.sensorRules[static_cast<uint8_t>(Sensors::CreatureEnergy)] = {"Creature Energy", 0, worldRules.creatureEnergyMax};
    worldRules.sensorRules[static_cast<uint8_t>(Sensors::CreatureRotation)] = {"Creature Rotation", 0, WorldDirectionCount};
    worldRules.sensorRules[static_cast<uint8_t>(Sensors::CreatureFatigue)] = {"Creature Fatigue", 0, worldRules.creatureFatigueMax};
    worldRules.sensorRules[static_cast<uint8_t>(Sensors::TouchArea_0_0)] = {"Touch Area 0-0", 0, TouchResultSize};
    worldRules.sensorRules[static_cast<uint8_t>(Sensors::TouchArea_0_1)] = {"Touch Area 0-1", 0, TouchResultSize};
    worldRules.sensorRules[static_cast<uint8_t>(Sensors::TouchArea_0_2)] = {"Touch Area 0-2", 0, TouchResultSize};
    worldRules.sensorRules[static_cast<uint8_t>(Sensors::TouchArea_1_0)] = {"Touch Area 1-0", 0, TouchResultSize};
    worldRules.sensorRules[static_cast<uint8_t>(Sensors::TouchArea_1_1)] = {"Touch Area 1-1", 0, TouchResultSize};
    worldRules.sensorRules[static_cast<uint8_t>(Sensors::TouchArea_1_2)] = {"Touch Area 1-2", 0, TouchResultSize};
    worldRules.sensorRules[static_cast<uint8_t>(Sensors::TouchArea_2_0)] = {"Touch Area 2-0", 0, TouchResultSize};
    worldRules.sensorRules[static_cast<uint8_t>(Sensors::TouchArea_2_1)] = {"Touch Area 2-1", 0, TouchResultSize};
    worldRules.sensorRules[static_cast<uint8_t>(Sensors::TouchArea_2_2)] = {"Touch Area 2-2", 0, TouchResultSize};

    static constexpr int32_t WorldSize = 100;
    {
        // init world
        std::vector cells{WorldSize * WorldSize, InvalidEcsEntity};
        for (int y = 0; y < WorldSize; ++y) {
            for (int x = 0; x < WorldSize; ++x) {
                const EcsEntity entity = world.create();
                world.emplace<WorldPositionComponent>(entity, static_cast<int16_t>(x), static_cast<int16_t>(y));
                cells[y * WorldSize + x] = entity;
            }
        }
        for (int y = 0; y < WorldSize; ++y) {
            for (int x = 0; x < WorldSize; ++x) {
                const EcsEntity worldEntity = cells[y * WorldSize + x];
                auto &[entities] = world.emplace<WorldAreaLocatorComponent>(worldEntity);

                for (int offsetX = -1; offsetX <= 1; ++offsetX) {
                    for (int offsetY = -1; offsetY <= 1; ++offsetY) {
                        const int32_t areaX = x + offsetX;
                        const int32_t areaY = y + offsetY;
                        EcsEntity worldAreaEntity = InvalidEcsEntity;
                        if (areaX >= 0 && areaX < WorldSize && areaY >= 0 && areaY < WorldSize) {
                            worldAreaEntity = cells[areaY * WorldSize + areaX];
                        }
                        const int32_t areaIndex = (offsetY + 1) * 3 + (offsetX + 1);
                        ASSERT(
                            areaIndex >= static_cast<int32_t>(WorldAreaIndex::Position_0_0) && areaIndex <= static_cast<int32_t>(WorldAreaIndex::Position_2_2),
                            "Sanity check: invalid area index");
                        entities[static_cast<uint8_t>(areaIndex)] = worldAreaEntity;
                    }
                }
            }
        }

        auto MakeObstacle = [&](const int x, const int y) {
            const EcsEntity border = cells[y * WorldSize + x];
            world.emplace_or_replace<WorldObstacleTag>(border);
        };

        for (int i = 0; i < WorldSize; ++i) {
            MakeObstacle(i, 0);
            MakeObstacle(i, WorldSize - 1);
            MakeObstacle(WorldSize - 1, i);
            MakeObstacle(0, i);
        }

        auto cellsToShuffle = cells;
        std::ranges::shuffle(cellsToShuffle, worldRules.randomGenerator);
        for (const EcsEntity position: cellsToShuffle | std::views::take((WorldSize * WorldSize) / 10)) {
            if (world.any_of<WorldCreatureComponent, WorldObstacleTag>(position)) {
                continue;
            }
            const EcsEntity creature = world.create();
            world.emplace<WorldCreatureComponent>(position, creature);
            InitCreatureGenome(worldRules, world, creature, position);
        }
    }


    /// Main loop
    bool shouldStopMainLoop = false;
    std::chrono::steady_clock::time_point lastFrameTime = std::chrono::steady_clock::now();
    static constexpr std::chrono::milliseconds TargetFrameTime{300};
    while (!shouldStopMainLoop) {
        const std::chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();
        const auto frameTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastFrameTime);
        lastFrameTime = currentTime;
        std::this_thread::sleep_for(TargetFrameTime - frameTime);

        SDL_Event event{};
        while (SDL_PollEvent(&event)) {
            const bool stopByQuitEvent = event.type == SDL_EVENT_QUIT;
            const bool stopByWindowEvent = event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED && event.window.windowID == SDL_GetWindowID(window);
            shouldStopMainLoop |= stopByQuitEvent || stopByWindowEvent;
        }

        ProcessWorldUpdate(world, worldRules);

        SDL_SetRenderDrawColor(renderer, 0xCC, 0xCC, 0xCC, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);
        rasterizationTarget.Lock();
        {
            world.view<const WorldPositionComponent, const WorldCreatureComponent>().each([&](const WorldPositionComponent &position, const WorldCreatureComponent &/*creature*/) {
                CellPosition p{position.position.x, position.position.y};
                rasterizationTarget.Set(p, SDL_Color{0, 200, 0, SDL_ALPHA_OPAQUE});
            });
        }
        rasterizationTarget.Unlock();
        if (!SDL_RenderTexture(renderer, renderTargetTexture, nullptr, nullptr)) {
            ASSERT_FAIL("Sanity check: failed to render texture");
            break;
        }
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(renderTargetTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
