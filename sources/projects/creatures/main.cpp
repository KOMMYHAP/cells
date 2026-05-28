#include "ecs_config.h"
#include "SDL3/SDL.h"

#include "imgui.h"
#include "backends/imgui_impl_sdl3.h"
#include "backends/imgui_impl_sdlrenderer3.h"

#include "brain.h"
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
    [[maybe_unused]] std::string_view name;
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
    const float rawValue = (static_cast<float>(value) - static_cast<float>(minValue)) / static_cast<float>(std::to_underlying(maxValue) -
                                                                                                           std::to_underlying(minValue));
    return std::clamp(rawValue, 0.0f, 1.0f);
}


enum class Genes : uint8_t {
    GeneCreatureMutationSpeedFactor,

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

    // GeneBrainRestSpeed,
    // GeneBitePower,

    InternalCount
};

static constexpr uint8_t GenesCount = static_cast<uint8_t>(Genes::InternalCount);


enum class GeneTypes {
    Ability, //< has or not?
    Property, //< how much?
    InternalCount
};

[[maybe_unused]] static constexpr uint8_t GeneTypesCount = static_cast<uint8_t>(GeneTypes::InternalCount);

struct GeneDescription {
    [[maybe_unused]] std::string_view name;
    GeneTypes type{GeneTypes::InternalCount};
    [[maybe_unused]] uint16_t minValue{0};
    [[maybe_unused]] uint16_t maxValue{0};
    [[maybe_unused]] float mutationCenter{0.0f};
    [[maybe_unused]] float mutationSigma{0.0f};
};

enum class AvailabilityResult {
    Yes [[maybe_unused]],
    No[[maybe_unused]],
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
    [[maybe_unused]] std::string_view name;
    int32_t energyCost{0};
    int32_t brainTickCost{0};
};

static constexpr uint8_t ActionTypeCount = static_cast<uint8_t>(Actions::InternalCount);


struct CreatureBrainConfig {
    static consteval size_t GetInputCount() { return SensorsCount; }
    static consteval size_t GetHiddenNeuronsCount() { return GenesCount; }
    static consteval size_t GetLinearOutputCount() { return 0; }
    static consteval size_t GetSigmoidOutputCount() { return ActionTypeCount; }
};

using CreatureBrain = BrainEvaluator<CreatureBrainConfig>;

struct WorldDescription {
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

using RandomGenerator = std::mt19937;

class Camera {
public: //< getters:
    float GetZoom() const {
        return _zoom;
    }

    std::pair<float, float> GetPosition() const {
        return {_centerX, _centerY};
    }

    std::pair<float, float> GetZoomLimits() const {
        return {_zoomMin, _zoomMax};
    }

    bool IsVisible(float x, float y) const {
        const auto [screenX, screenY] = ToScreenSpace(x, y);
        return screenX >= 0 && screenY >= 0 && screenX < _screenPixelsWidth && screenY < _screenPixelsHeight;
    }

public: //< converter:
    std::pair<int32_t, int32_t> ToScreenSpace(float x, float y) const {
        const auto screenX = static_cast<int32_t>(std::round((x - _centerX) * _zoom + _screenPixelsWidth / 2.0f));
        const auto screenY = static_cast<int32_t>(std::round((y - _centerY) * _zoom + _screenPixelsHeight / 2.0f));
        return {screenX, screenY};
    }

    std::pair<float, float> ToWorldSpace(int32_t x, int32_t y) const {
        const float worldX = static_cast<float>(x - _screenPixelsWidth / 2) / _zoom - _centerX;
        const float worldY = static_cast<float>(y - _screenPixelsHeight / 2) / _zoom - _centerY;
        return {worldX, worldY};
    }

public: //< controller:
    void SetZoomLimits(float zoomMin, float zoomMax) {
        _zoomMin = zoomMin;
        _zoomMax = zoomMax;
    }

    void SetZoom(float zoom) {
        _zoom = std::clamp(zoom, _zoomMin, _zoomMax);
    }

    void SetPosition(float x, float y) {
        _centerX = x;
        _centerY = y;
    }

    void SetScreenSize(int32_t width, int32_t height) {
        ASSERT(width > 0 && height > 0, "Sanity check: screen size must be positive");
        _screenPixelsWidth = width;
        _screenPixelsHeight = height;
    }

    void Zoom(int32_t screenSpaceX, int32_t screenSpaceY, float zoomDelta) {
        const auto [oldWorldX, oldWorldY] = ToWorldSpace(screenSpaceX, screenSpaceY);
        SetZoom(_zoom * zoomDelta);
        const auto [newWorldX, newWorldY] = ToWorldSpace(screenSpaceX, screenSpaceY);
        _centerX += newWorldX - oldWorldX;
        _centerY += newWorldY - oldWorldY;
    }

    void Move(float screenSpaceDx, float screenSpaceDy) {
        _centerX += screenSpaceDx / _zoom;
        _centerY += screenSpaceDy / _zoom;
    }

private:
    float _centerX{0.0f}; //< world space X
    float _centerY{0.0f}; //< world space Y

    float _zoomMin{0.1f};
    float _zoomMax{10.0f};
    float _zoom{1.0f};

    int32_t _screenPixelsWidth{800};
    int32_t _screenPixelsHeight{600};
};

struct GameContext {
    WorldDescription worldRules;
    Camera camera;
    RandomGenerator randomGenerator;

    std::optional<float> prevMousePosX;
    std::optional<float> prevMousePosY;
};

struct CreatureSensorsComponent {
    std::array<float, SensorsCount> values;
};

struct CreatureGenomeComponent {
    std::array<float, GenesCount> quality;
};

struct CreatureBrainNeuronsComponent {
    CreatureBrain::BrainNeurons values;
};

struct BrainReactionComponent {
    CreatureBrain::BrainOutput values; //< (0; 1)
};

struct BrainContext {
    ConstRef<CreatureSensorsComponent> sensors;
    ConstRef<CreatureGenomeComponent> genome;
    ConstRef<CreatureBrainNeuronsComponent> specialization;
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

struct WorldPosition {
    int16_t x{0};
    int16_t y{0};
};

enum class WorldAreaIndex : uint8_t {
    Position_0_0,
    Position_0_1,
    Position_0_2[[maybe_unused]],
    Position_1_0,
    Position_1_1[[maybe_unused]],
    Position_1_2,
    Position_2_0[[maybe_unused]],
    Position_2_1,
    Position_2_2,
    InternalCount
};

static constexpr int32_t WorldAreaSize = static_cast<int32_t>(WorldAreaIndex::InternalCount);

struct WorldAreaLocatorComponent {
    [[maybe_unused]] std::array<EcsEntity, WorldAreaSize> entities;
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
    float _amplitude{0.0f};
};


bool TryConsumeEnergy(EcsWorld &world, const WorldDescription &worldDesc, EcsEntity creature, CreatureStateEnergyComponent &energy, Actions action,
                      float amplitude = 1.0f) {
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


void InitCreatureGenome(EcsWorld &world, EcsEntity creature, EcsEntity position) {
    GameContext &context = world.ctx().get<GameContext>();
    RandomGenerator &randomGenerator = context.randomGenerator;
    const WorldDescription &worldRules = context.worldRules;

    const int32_t initialEnergy = std::uniform_int_distribution{0, worldRules.creatureEnergyMax - 1}(randomGenerator);
    const int32_t initialFatigue = std::uniform_int_distribution{0, worldRules.creatureFatigueMax - 1}(randomGenerator);
    const int32_t initialRotation = std::uniform_int_distribution{0, WorldDirectionCount - 1}(randomGenerator);

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
    actions.sigmoidValues.fill(0.0f);
    actions.linearValues.fill(0.0f);

    auto &[sensors] = world.emplace<CreatureSensorsComponent>(creature);
    sensors.fill(0.0f);

    auto FillBrain = [&]<size_t S>(std::array<float, S> &values) {
        for (float &value: values) {
            value = std::uniform_real_distribution{-worldRules.neuronValueRangeSize / 2.0f, worldRules.neuronValueRangeSize / 2.0f}(context.randomGenerator);
        }
    };
    auto &neurons = world.emplace<CreatureBrainNeuronsComponent>(creature);
    FillBrain(neurons.values.biasInputToHidden);
    FillBrain(neurons.values.weightInputToHidden);
    FillBrain(neurons.values.biasHiddenToOutput);
    FillBrain(neurons.values.weightHiddenToOutput);

    auto &[genomeQuality] = world.emplace<CreatureGenomeComponent>(creature);
    for (float &quality: genomeQuality) {
        quality = std::uniform_real_distribution{0.0f, 1.0f}(context.randomGenerator);
    }
}

void ProcessWorldUpdate(EcsWorld &world) {
    GameContext &context = world.ctx().get<GameContext>();
    const WorldDescription &worldRules = context.worldRules;

    world.view<CreatureBrainNeuronsComponent, const CreatureGenomeComponent>().each(
        [&](CreatureBrainNeuronsComponent &neurons, const CreatureGenomeComponent &genome) {
            static constexpr auto MutationSpeedGeneIndex = static_cast<uint8_t>(Genes::GeneCreatureMutationSpeedFactor);
            static constexpr float MutationSpeedFactorMin = 0.0001f;
            static constexpr float MutationSpeedFactorMax = 1.0f;
            const float mutationSigma = genome.quality[MutationSpeedGeneIndex] * (MutationSpeedFactorMax - MutationSpeedFactorMin) + MutationSpeedFactorMin;

            auto MutateNeuron = [&context, range=worldRules.neuronValueRangeSize, sigma=mutationSigma]<size_t N>(std::array<float, N> &values) {
                for (float &value: values) {
                    const float neuronMutation = std::normal_distribution{0.0f, sigma}(context.randomGenerator);
                    value += neuronMutation;
                    value = std::clamp(value, -range / 2.0f, range / 2.0f);
                }
            };

            MutateNeuron(neurons.values.biasInputToHidden);
            MutateNeuron(neurons.values.weightInputToHidden);
            MutateNeuron(neurons.values.biasHiddenToOutput);
            MutateNeuron(neurons.values.weightHiddenToOutput);
        });

    world.view<CreatureSensorsComponent, const CreatureSensorEnergyComponent, const CreatureSensorRotationComponent, const CreatureSensorTouchComponent>().each(
        [&](CreatureSensorsComponent &sensors, const CreatureSensorEnergyComponent energySensor, const CreatureSensorRotationComponent rotationSensor,
            const CreatureSensorTouchComponent &touchSensor) {
            sensors.values[static_cast<uint8_t>(Sensors::CreatureEnergy)] = ConvertToSensorValue(energySensor.value,
                                                                                                 worldRules.sensorRules[static_cast<uint8_t>(Sensors::CreatureEnergy)]);
            sensors.values[static_cast<uint8_t>(Sensors::CreatureRotation)] = ConvertToSensorValue(rotationSensor.value);
            sensors.values[static_cast<uint8_t>(Sensors::CreatureFatigue)] = ConvertToSensorValue(energySensor.value,
                                                                                                  worldRules.sensorRules[static_cast<uint8_t>(Sensors::CreatureFatigue)]);

            for (const int32_t touchIndex: std::views::iota(0, WorldAreaSize)) {
                const TouchResult touch = touchSensor.touches[touchIndex];
                sensors.values[static_cast<int32_t>(Sensors::TouchArea_0_0) + touchIndex] = ConvertToSensorValue(touch);
            }
        });

    world.view<const CreatureSensorsComponent, const CreatureBrainNeuronsComponent, BrainReactionComponent>().each(
        [&](const CreatureSensorsComponent &sensors, const CreatureBrainNeuronsComponent &neurons, BrainReactionComponent &output) {
            CreatureBrain::BrainInput input;
            input.values = sensors.values;

            static constexpr CreatureBrain brain;
            brain.Evaluate(ConstRef{&input}, ConstRef{&neurons.values}, Ref{&output.values});
        });

    auto BrainDispatchAction = [&](EcsEntity creature, const Actions action, float intent) {
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
                world.emplace<CreatureActionBiteComponent>(creature, intent);
                break;
            case Actions::Move: {
                const auto direction = ConvertFromSensorValue<WorldDirection>(intent);
                world.emplace<CreatureActionMoveComponent>(creature, direction);
            }
            break;
            case Actions::Rotate: {
                const auto direction = ConvertFromSensorValue<WorldDirection>(intent);
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

    world.view<const BrainReactionComponent, CreatureBrainReactionStateComponent>(entt::exclude_t<CreatureBrainIsOverloadedTag>{}).each(
        [&](EcsEntity creature, const BrainReactionComponent &reaction, CreatureBrainReactionStateComponent &brainState) {
            int32_t brainTickRequested = 0;
            for (uint8_t actionIndex = 0; actionIndex < ActionTypeCount; ++actionIndex) {
                const float intent = reaction.values.sigmoidValues[actionIndex];
                BrainDispatchAction(creature, static_cast<Actions>(actionIndex), intent);
                brainTickRequested += worldRules.actionRules[actionIndex].brainTickCost;
            }

            brainState.brainTicksConsumed = static_cast<uint16_t>(brainTickRequested);
        });

    world.view<const CreatureBrainReactionStateComponent, CreatureStateFatigueComponent>(entt::exclude_t<CreatureBrainIsOverloadedTag>{}).each(
        [&](EcsEntity creature, const CreatureBrainReactionStateComponent &brainState, CreatureStateFatigueComponent &fatigue) {
            const SensorDescription &desc = worldRules.sensorRules[static_cast<uint8_t>(Sensors::CreatureFatigue)];
            const SensorCalculationResult r = UpdateSensorValue(desc, fatigue.value, brainState.brainTicksConsumed);
            if (r == SensorCalculationResult::MaxValueReached) {
                world.emplace<CreatureBrainIsOverloadedTag>(creature);
            }
        });
    {
        const auto overloadedBrains = world.view<const CreatureBrainIsOverloadedTag, const CreaturePositionComponent>();
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

    world.view<const CreatureActionRotateComponent, CreatureStateRotationComponent, CreatureStateEnergyComponent>(entt::exclude_t<CreatureOutOfEnergyTag>{}).
            each([&](EcsEntity creature, const CreatureActionRotateComponent &rotate, CreatureStateRotationComponent &rotation,
                     CreatureStateEnergyComponent &energy) {
                const SensorDescription &desc = worldRules.sensorRules[static_cast<uint8_t>(Sensors::CreatureEnergy)];
                const ActionDescription &actionDesc = worldRules.actionRules[static_cast<uint8_t>(Actions::Rotate)];
                const SensorCalculationResult r = UpdateSensorValue(desc, energy.value, -actionDesc.energyCost);
                if (r == SensorCalculationResult::MinValueReached) {
                    world.emplace<CreatureOutOfEnergyTag>(creature);
                }

                world.erase<CreatureActionRotateComponent>(creature);
                rotation.value = rotate.value;
            });

    world.view<const CreatureActionMoveComponent, CreaturePositionComponent, CreatureStateEnergyComponent>(entt::exclude_t<CreatureOutOfEnergyTag>{}).each(
        [&](EcsEntity creature, const CreatureActionMoveComponent &move, CreaturePositionComponent &position, CreatureStateEnergyComponent &energy) {
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

    world.view<const CreatureActionReadTouchAreaTag, const CreaturePositionComponent, CreatureSensorTouchComponent,
        CreatureStateEnergyComponent>(entt::exclude_t<CreatureOutOfEnergyTag>{}).each(
        [&](EcsEntity creature, const CreaturePositionComponent &creaturePosition, CreatureSensorTouchComponent &touchSensor,
            CreatureStateEnergyComponent &energy) {
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

    world.view<const CreatureActionReadEnergyTag, CreatureStateEnergyComponent, CreatureSensorEnergyComponent>(entt::exclude_t<CreatureOutOfEnergyTag>{}).each(
        [&](EcsEntity creature, CreatureStateEnergyComponent &energy, CreatureSensorEnergyComponent &energySensor) {
            world.erase<CreatureActionReadEnergyTag>(creature);
            if (!TryConsumeEnergy(world, worldRules, creature, energy, Actions::ReadCreatureEnergy)) {
                return;
            }
            energySensor.value = energy.value;
        });

    world.view<const CreatureActionReadRotationTag, const CreatureStateRotationComponent, CreatureSensorRotationComponent,
        CreatureStateEnergyComponent>(entt::exclude_t<CreatureOutOfEnergyTag>{}).each(
        [&](EcsEntity creature, const CreatureStateRotationComponent &rotation, CreatureSensorRotationComponent &rotationSensor,
            CreatureStateEnergyComponent &energy) {
            world.erase<CreatureActionReadRotationTag>(creature);
            if (!TryConsumeEnergy(world, worldRules, creature, energy, Actions::ReadCreatureRotation)) {
                return;
            }
            rotationSensor.value = rotation.value;
        });

    world.view<const CreatureActionBiteComponent, CreatureStateEnergyComponent, const CreatureStateRotationComponent, const
        CreaturePositionComponent>(entt::exclude_t<CreatureOutOfEnergyTag>{}).each(
        [&](EcsEntity creature, const CreatureActionBiteComponent bite, CreatureStateEnergyComponent &energy, const CreatureStateRotationComponent rotation,
            const CreaturePositionComponent position) {
            world.erase<CreatureActionBiteComponent>(creature);
            if (!TryConsumeEnergy(world, worldRules, creature, energy, Actions::Bite, bite._amplitude)) {
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
        const auto creaturesToDestroy = world.view<const CreatureOutOfEnergyTag, const CreaturePositionComponent>();
        creaturesToDestroy.each([&](const CreaturePositionComponent position) {
            // cleanup reference from world to creature
            world.erase<WorldCreatureComponent>(position.value);
        });
        world.destroy(creaturesToDestroy.begin(), creaturesToDestroy.end());
    }
}

void FillWorldContent(EcsWorld &world, GameContext &gameContext) {
    static constexpr int32_t WorldSize = 100;

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
    std::ranges::shuffle(cellsToShuffle, gameContext.randomGenerator);
    for (const EcsEntity position: cellsToShuffle | std::views::take(cells.size() / 10)) {
        if (world.any_of<WorldCreatureComponent, WorldObstacleTag>(position)) {
            continue;
        }
        const EcsEntity creature = world.create();
        world.emplace<WorldCreatureComponent>(position, creature);
        InitCreatureGenome(world, creature, position);
    }
}

void SetupWorldRules(const int32_t &ScreenWidth, const int32_t &ScreenHeight, GameContext &gameContext) {
    WorldDescription &worldRules = gameContext.worldRules;
    {
        static constexpr std::string_view RandomSeed = "White";
        std::seed_seq seed{RandomSeed.begin(), RandomSeed.end()};
        gameContext.randomGenerator.seed(seed);
    }

    gameContext.camera.SetScreenSize(ScreenWidth, ScreenHeight);

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

    worldRules.geneRules[static_cast<uint8_t>(Genes::GeneCreatureMutationSpeedFactor)] = {
        "Mutation Speed", GeneTypes::Property, 0, 0, std::numeric_limits<float>::signaling_NaN(), std::numeric_limits<float>::signaling_NaN()
    };

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
}

void ProcessImGui(std::chrono::milliseconds elapsedTime, EcsWorld &world) {
    GameContext &context = world.ctx().get<GameContext>();
    const ImGuiIO &io = ImGui::GetIO();
    const float mouseWheelDelta = io.MouseWheel;
    const float mousePosX = io.MousePos.x;
    const float mousePosY = io.MousePos.y;
    const float mouseDeltaX = mousePosX - context.prevMousePosX.value_or(mousePosX);
    const float mouseDeltaY = mousePosY - context.prevMousePosY.value_or(mousePosY);
    context.prevMousePosX = mousePosX;
    context.prevMousePosY = mousePosY;
    const int32_t mousePosScreenSpaceX = static_cast<int32_t>(std::round(mousePosX));
    const int32_t mousePosScreenSpaceY = static_cast<int32_t>(std::round(mousePosY));

    // Update camera:
    if (std::abs(mouseWheelDelta) > 0.001f) {
        context.camera.Zoom(mousePosScreenSpaceX, mousePosScreenSpaceY, mouseWheelDelta);
    }
    if (io.MouseDown[ImGuiMouseButton_Left] && !ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow)) {
        if (std::abs(mouseDeltaX) > 0.001f || std::abs(mouseDeltaY) > 0.001f) {
            context.camera.Move(mouseDeltaX, mouseDeltaY);
        }
    }
    const
            auto [cameraPositionX, cameraPositionY] = context.camera.GetPosition();

    if (ImGui::BeginMainMenuBar()) {
        static bool demoWindowOpened{false};
        if (ImGui::MenuItem("Demo", nullptr, &demoWindowOpened)) {
            ImGui::ShowDemoWindow(&demoWindowOpened);
        }
        ImGui::EndMainMenuBar();
    }

    // Status window:
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.8f, 0.8f, 0.8f, 0.65f));
    ImGui::SetNextWindowSize(ImVec2(300, 100), ImGuiCond_FirstUseEver);
    ImGui::Begin("##status_window", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse);
    ImGui::Text("FPS: %3.0f (%03d ms)", 1000.0f / elapsedTime.count(), static_cast<int32_t>(elapsedTime.count()));
    ImGui::Text("Mouse: position = (%.0f, %.0f), scroll = %.0f", mousePosX, mousePosY, mouseWheelDelta);
    ImGui::Text("Camera: position = (%.0f, %.0f), zoom = %.0f", cameraPositionX, cameraPositionY, context.camera.GetZoom());
    ImGui::End();
    ImGui::PopStyleColor();
}

int main() {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        ASSERT_FAIL("SDL_Init failed");
        return -1;
    }

    static constexpr uint32_t WindowFlags = SDL_WINDOW_HIGH_PIXEL_DENSITY;
    static constexpr int32_t ScreenWidth = 1200;
    static constexpr int32_t ScreenHeight = 800;
    SDL_Window *window = SDL_CreateWindow("Creatures", ScreenWidth, ScreenHeight, WindowFlags);
    if (window == nullptr) {
        ASSERT_FAIL("SDL_CreateWindow failed");
        return -1;
    }

    const SDL_PropertiesID props = SDL_CreateProperties();
    SDL_SetPointerProperty(props, SDL_PROP_RENDERER_CREATE_WINDOW_POINTER, window);
    SDL_SetNumberProperty(props, SDL_PROP_RENDERER_CREATE_PRESENT_VSYNC_NUMBER, 1);
    SDL_Renderer *renderer = SDL_CreateRendererWithProperties(props);
    SDL_DestroyProperties(props);

    if (renderer == nullptr) {
        std::fprintf(stderr, "SDL_CreateRenderer failed: %s\n", SDL_GetError());
        ASSERT_FAIL("SDL_CreateRenderer failed");
        return -1;
    }

    SDL_Texture *renderTargetTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, ScreenWidth, ScreenHeight);
    if (!renderTargetTexture) {
        ASSERT_FAIL("SDL_CreateTexture failed");
        return -1;
    }

    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls

    if (!ImGui_ImplSDL3_InitForSDLRenderer(window, renderer)) {
        ASSERT_FAIL("ImGui_ImplSDL2_InitForSDLRenderer failed!");
        return -1;
    }
    if (!ImGui_ImplSDLRenderer3_Init(renderer)) {
        ASSERT_FAIL("ImGui_ImplSDLRenderer2_Init failed!");
        return -1;
    }

    WorldRasterizationTarget rasterizationTarget{*renderTargetTexture, SDL_Color{200, 200, 200, SDL_ALPHA_OPAQUE}, 4};

    EcsWorld world{};
    GameContext &gameContext = world.ctx().emplace<GameContext>();
    SetupWorldRules(ScreenWidth, ScreenHeight, gameContext);
    FillWorldContent(world, gameContext);

    /// Main loop
    bool shouldStopMainLoop = false;
    std::chrono::steady_clock::time_point lastFrameTime = std::chrono::steady_clock::now();
    static constexpr std::chrono::milliseconds TargetFrameTime{1000 / 60};
    while (!shouldStopMainLoop) {
        const std::chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();
        auto elapsedFrameTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastFrameTime);
        lastFrameTime = currentTime;
        // if (!vsyncChanged && elapsedFrameTime < TargetFrameTime) {
        //     std::this_thread::sleep_for(TargetFrameTime - elapsedFrameTime);
        //     elapsedFrameTime = TargetFrameTime;
        // }

        SDL_Event event{};
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL3_ProcessEvent(&event);
            const bool stopByQuitEvent = event.type == SDL_EVENT_QUIT;
            const bool stopByWindowEvent = event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED && event.window.windowID == SDL_GetWindowID(window);
            shouldStopMainLoop |= stopByQuitEvent || stopByWindowEvent;
        }

        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();
        ProcessImGui(elapsedFrameTime, world);
        ImGui::Render();

        ProcessWorldUpdate(world);

        SDL_SetRenderDrawColor(renderer, 0xCC, 0xCC, 0xCC, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);
        rasterizationTarget.Lock();
        {
            world.view<const WorldPositionComponent, const WorldCreatureComponent>().each(
                [&](const WorldPositionComponent &position, const WorldCreatureComponent &/*creature*/) {
                    static constexpr float CreatureWorldSpaceRadius = 5.0f;
                    const float creatureScreenSpaceRadius = CreatureWorldSpaceRadius * gameContext.camera.GetZoom();
                    if (!gameContext.camera.IsVisible(position.position.x, position.position.y)) {
                        return;
                    }
                    const auto [screenSpaceX, screenSpaceY] = gameContext.camera.ToScreenSpace(position.position.x, position.position.y);
                    rasterizationTarget.SetFilledCircle(screenSpaceX, screenSpaceY, creatureScreenSpaceRadius, SDL_Color{0, 200, 0, SDL_ALPHA_OPAQUE});
                });
        }
        rasterizationTarget.Unlock();
        if (!SDL_RenderTexture(renderer, renderTargetTexture, nullptr, nullptr)) {
            ASSERT_FAIL("Sanity check: failed to render texture");
            break;
        }
        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);
        SDL_RenderPresent(renderer);
    }

    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyTexture(renderTargetTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
