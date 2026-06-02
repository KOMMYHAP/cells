#include <numbers>

#include "ecs_config.h"
#include "SDL3/SDL.h"

#include "imgui.h"
#include "backends/imgui_impl_sdl3.h"
#include "backends/imgui_impl_sdlrenderer3.h"

#include "brain.h"
#include "widgets/world/world_rasterization_target.h"

#include "box2d/box2d.h"
#include "glm/glm.hpp"


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
    ReadCreatureEnergy,
    // ReadCreatureRotation,

    // ReadAreaSound,
    // ReadAreaTouch,
    // ReadAreaSmell,
    // ReadAreaPhoto,
    // ReadAreaThermo,

    // Bite,
    MoveVelocity,
    MoveIntent,
    // Rotate,

    // MakeSound,
    // MakeSmell,
    // MakeTemperature,
    // MakeFlash,
    // MakeChild,

    InternalCount
};

struct LinearActionType {
    float activationThreshold{0.0f};
    float valueLimit{0.0f};
    bool symmetric{false};
};

struct SigmoidActionType {
    float activationThreshold{0.0f};
};


struct ActionDescription {
    [[maybe_unused]] std::string_view name;
    int32_t energyCost{0};
    int32_t brainTickCost{0};
};

static constexpr uint8_t ActionTypeCount = static_cast<uint8_t>(Actions::InternalCount);


struct CreatureBrainConfig {
    static consteval size_t GetInputCount() {
        return SensorsCount + GenesCount;
    }

    static consteval size_t GetHiddenNeuronsCount() {
        return GenesCount;
    }

    static consteval size_t GetLinearOutputCount() {
        return 0;
    }

    static consteval size_t GetSigmoidOutputCount() {
        return ActionTypeCount;
    }
};

using CreatureBrain = BrainEvaluator<CreatureBrainConfig>;

struct WorldDescription {
    std::array<ActionDescription, ActionTypeCount> actionRules{};
    std::array<GeneDescription, GenesCount> geneRules{};
    std::array<SensorDescription, SensorsCount> sensorRules{};
    std::array<std::optional<LinearActionType>, ActionTypeCount> linearActions{};
    std::array<std::optional<SigmoidActionType>, ActionTypeCount> sigmoidActions{};

    int32_t worldSizeX{0};
    int32_t worldSizeY{0};
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

    std::pair<float, float> GetSize() const {
        return {_screenPixelsWidth / _zoom, _screenPixelsHeight / _zoom};
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
        const float worldX = static_cast<float>(x - _screenPixelsWidth / 2) / _zoom + _centerX;
        const float worldY = static_cast<float>(y - _screenPixelsHeight / 2) / _zoom + _centerY;
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
        SetZoom(_zoom + zoomDelta);
        const auto [newWorldX, newWorldY] = ToWorldSpace(screenSpaceX, screenSpaceY);
        _centerX -= newWorldX - oldWorldX;
        _centerY -= newWorldY - oldWorldY;
    }

    void Move(float screenSpaceDx, float screenSpaceDy) {
        _centerX -= screenSpaceDx / _zoom;
        _centerY -= screenSpaceDy / _zoom;
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

class WorldLocator {
public:
    WorldLocator() = default;

    WorldLocator(int32_t worldSizeX, int32_t worldSizeY);

    EcsEntity Find(float x, float y) const;

    bool TryReset(EcsEntity entity, float x, float y);

    bool TrySet(EcsEntity entity, float x, float y);

    bool TryMove(EcsEntity entity, float xFrom, float yFrom, float xTo, float yTo);

private:
    int32_t ToCellIndex(float x, float y) const;

    int32_t worldSizeX{0};
    std::vector<EcsEntity> _cells;
};

int32_t WorldLocator::ToCellIndex(float x, float y) const {
    const int32_t cellX = static_cast<int32_t>(std::round(x));
    const int32_t cellY = static_cast<int32_t>(std::round(y));
    const int32_t index = cellY * worldSizeX + cellX;
    if (index >= 0 && index < _cells.size()) {
        return index;
    }
    return -1;
}

WorldLocator::WorldLocator(int32_t worldSizeX, int32_t worldSizeY)
    : _cells(worldSizeX * worldSizeY, InvalidEcsEntity) {
}

EcsEntity WorldLocator::Find(float x, float y) const {
    const int32_t index = ToCellIndex(x, y);
    if (index < 0) {
        return InvalidEcsEntity;
    }
    return _cells[index];
}

bool WorldLocator::TrySet(EcsEntity entity, float x, float y) {
    const int32_t index = ToCellIndex(x, y);
    if (index < 0) {
        return false;
    }

    if (_cells[index] != InvalidEcsEntity) {
        return false;
    }

    _cells[index] = entity;
    return true;
}

bool WorldLocator::TryReset(EcsEntity entity, float x, float y) {
    const int32_t index = ToCellIndex(x, y);
    if (index < 0) {
        return false;
    }

    if (_cells[index] != entity) {
        return false;
    }

    _cells[index] = InvalidEcsEntity;
    return true;
}

bool WorldLocator::TryMove(EcsEntity entity, float xFrom, float yFrom, float xTo, float yTo) {
    const int32_t indexFrom = ToCellIndex(xFrom, yFrom);
    const int32_t indexTo = ToCellIndex(xTo, yTo);
    if (indexFrom < 0 || indexTo < 0) {
        return false;
    }
    if (_cells[indexFrom] != entity) {
        return false;
    }
    if (_cells[indexTo] != InvalidEcsEntity) {
        return false;
    }
    _cells[indexTo] = std::exchange(_cells[indexFrom], InvalidEcsEntity);
    return true;
}


struct GameContext {
    b2WorldId physicsWorld{};
    WorldLocator worldLocator;
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


enum class WorldDirection : uint8_t {
    Up,
    Down,
    Left,
    Right,
    InternalCount
};

static constexpr uint8_t WorldDirectionCount = static_cast<uint8_t>(WorldDirection::InternalCount);


enum class TouchResult : uint8_t {
    Nothing,
    // Fluid,
    // Sticky,
    Elastic,
    Solid,
    InternalCount
};

static constexpr int32_t TouchResultSize = static_cast<int32_t>(TouchResult::InternalCount);

struct CreatureBrainIsOverloadedTag {
};

struct CreatureBrainReactionStateComponent {
    uint16_t brainTicksConsumed{0};
};

struct CreatureStateEnergyComponent {
    uint16_t value{0};
};

struct CreatureOutOfEnergyTag {
};

struct CreatureStateFatigueComponent {
    uint16_t value{0};
};

struct CreatureStateAngularVelocityComponent {
    float angularVelocity{0.0f}; //< radians per second
};

struct CreatureActionRotateComponent {
    float angularVelocity{0.0f};
};

struct CreatureActionMoveTag {
};

struct CreatureActionSpeedComponent {
    float speed{0.0f};
};

struct CreatureSensorTouchComponent {
    std::array<TouchResult, 3> touches; //< left, forward, up
};

struct CreatureActionReadTouchAreaTag {
};

struct CreatureSensorEnergyComponent {
    uint16_t value{0};
};

struct CreatureActionReadEnergyTag {
};

struct CreatureSensorRotationComponent {
    float angle{0.0f}; //< radians, [0; 2 * PI)
};

struct CreatureActionReadRotationTag {
};

struct WorldCreatureTag {
};

struct PhysicsBodyComponent {
    b2BodyId id{};
};

struct CreatureActionBiteComponent {
    float _amplitude{0.0f};
};

struct RenderDirtyTag {
};

bool TryConsumeEnergy(EcsWorld &world, const WorldDescription &worldDesc, EcsEntity creature, CreatureStateEnergyComponent &energy, Actions action,
                      float amplitude = 1.0f) {
    const SensorDescription &desc = worldDesc.sensorRules[static_cast<uint8_t>(Sensors::CreatureEnergy)];
    const ActionDescription &actionDesc = worldDesc.actionRules[static_cast<uint8_t>(action)];
    const int32_t energyCost = std::max(1, static_cast<int32_t>(std::round(static_cast<float>(actionDesc.energyCost) * amplitude)));
    const SensorCalculationResult r = UpdateSensorValue(desc, energy.value, -energyCost);
    if (r == SensorCalculationResult::MinValueReached) {
        // world.emplace<CreatureOutOfEnergyTag>(creature);
        return false;
    }
    return true;
}


void InitCreatureGenome(EcsWorld &world, EcsEntity creature) {
    GameContext &context = world.ctx().get<GameContext>();
    RandomGenerator &randomGenerator = context.randomGenerator;
    const WorldDescription &worldRules = context.worldRules;

    const int32_t initialEnergy = std::uniform_int_distribution{0, worldRules.creatureEnergyMax - 1}(randomGenerator);
    const int32_t initialFatigue = std::uniform_int_distribution{0, worldRules.creatureFatigueMax - 1}(randomGenerator);
    // const float initialRotation = std::uniform_real_distribution<float>{}(randomGenerator);

    world.emplace<CreatureStateEnergyComponent>(creature, static_cast<uint16_t>(initialEnergy));
    world.emplace<CreatureStateFatigueComponent>(creature, static_cast<uint16_t>(initialFatigue));
    // world.emplace<CreatureStateAngularVelocityComponent>(creature, static_cast<WorldDirection>(initialRotation));
    world.emplace<CreatureBrainReactionStateComponent>(creature, uint16_t{0});

    world.emplace<CreatureSensorEnergyComponent>(creature, static_cast<uint16_t>(initialEnergy));
    // world.emplace<CreatureSensorRotationComponent>(creature, initialRotation);
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
        [&](CreatureSensorsComponent &sensors, const CreatureSensorEnergyComponent energySensor, const CreatureSensorRotationComponent /*rotationSensor*/, const CreatureSensorTouchComponent &touchSensor) {
            sensors.values[static_cast<uint8_t>(Sensors::CreatureEnergy)] = ConvertToSensorValue(energySensor.value,
                                                                                                 worldRules.sensorRules[static_cast<uint8_t>(Sensors::CreatureEnergy)]);
            // sensors.values[static_cast<uint8_t>(Sensors::CreatureRotation)] = ConvertToSensorValue(rotationSensor.angle,
            //                                                                                        worldRules.sensorRules[static_cast<uint8_t>(Sensors::CreatureRotation)]);
            sensors.values[static_cast<uint8_t>(Sensors::CreatureFatigue)] = ConvertToSensorValue(energySensor.value,
                                                                                                  worldRules.sensorRules[static_cast<uint8_t>(Sensors::CreatureFatigue)]);

            for (const int32_t touchIndex: std::views::iota(0, static_cast<int32_t>(touchSensor.touches.size()))) {
                const TouchResult touch = touchSensor.touches[touchIndex];
                sensors.values[static_cast<int32_t>(Sensors::TouchArea_0_0) + touchIndex] = ConvertToSensorValue(touch);
            }
        });

    world.view<const CreatureSensorsComponent, const CreatureGenomeComponent, const CreatureBrainNeuronsComponent, BrainReactionComponent>().each(
        [&](const CreatureSensorsComponent &sensors, const CreatureGenomeComponent &genome, const CreatureBrainNeuronsComponent &neurons,
            BrainReactionComponent &output) {
            CreatureBrain::BrainInput input;
            std::ranges::copy(sensors.values, std::ranges::begin(input.values));
            std::ranges::copy(genome.quality, std::ranges::begin(input.values) + sensors.values.size());

            static constexpr CreatureBrain brain;
            brain.Evaluate(ConstRef{&input}, ConstRef{&neurons.values}, Ref{&output.values});
        });

    auto BrainDispatchAction = [&](EcsEntity creature, const Actions action, float value) {
        switch (action) {
            case Actions::ReadCreatureEnergy:
                world.emplace<CreatureActionReadEnergyTag>(creature);
                break;
            // case Actions::ReadCreatureRotation:
            //     world.emplace<CreatureActionReadRotationTag>(creature);
            //     break;
            // case Actions::ReadAreaTouch:
            //     world.emplace<CreatureActionReadTouchAreaTag>(creature);
            //     break;
            // case Actions::Bite:
            //     // world.emplace<CreatureActionBiteComponent>(creature, intent);
            //     break;
            case Actions::MoveVelocity: {
                world.emplace<CreatureActionSpeedComponent>(creature, value);
                break;
            }
            case Actions::MoveIntent: {
                world.emplace<CreatureActionMoveTag>(creature);
                break;
            }
            break;
                // case Actions::Rotate: {
                // const auto direction = ConvertFromSensorValue<WorldDirection>(intent);
                // world.emplace<CreatureActionRotateComponent>(creature, direction);
                // }
                break;
            // case Actions::MakeChild:
            //     break;
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
        const auto overloadedBrains = world.view<const CreatureBrainIsOverloadedTag>();
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

    world.view<const CreatureActionRotateComponent, PhysicsBodyComponent, CreatureStateAngularVelocityComponent, CreatureStateEnergyComponent>(entt::exclude_t<CreatureOutOfEnergyTag>{}).
            each([&](EcsEntity creature, const CreatureActionRotateComponent &rotate, PhysicsBodyComponent physicsBody, CreatureStateAngularVelocityComponent &rotation, CreatureStateEnergyComponent &/*energy*/) {
                // const SensorDescription &desc = worldRules.sensorRules[static_cast<uint8_t>(Sensors::CreatureEnergy)];
                // const ActionDescription &actionDesc = worldRules.actionRules[static_cast<uint8_t>(Actions::Rotate)];
                // const SensorCalculationResult r = UpdateSensorValue(desc, energy.value, -actionDesc.energyCost);
                // if (r == SensorCalculationResult::MinValueReached) {
                //     world.emplace<CreatureOutOfEnergyTag>(creature);
                // }

                world.erase<CreatureActionRotateComponent>(creature);
                rotation.angularVelocity = rotate.angularVelocity;
                b2Body_SetAngularVelocity(physicsBody.id, rotation.angularVelocity);
            });

    world.view<const CreatureActionMoveTag, const CreatureActionSpeedComponent, PhysicsBodyComponent, CreatureStateEnergyComponent>(entt::exclude_t<CreatureOutOfEnergyTag>{}).each(
        [&](EcsEntity creature, const CreatureActionSpeedComponent &move, PhysicsBodyComponent physicsBody, CreatureStateEnergyComponent &energy) {
            world.erase<CreatureActionSpeedComponent, CreatureActionMoveTag>(creature);
            if (!TryConsumeEnergy(world, worldRules, creature, energy, Actions::MoveVelocity)) {
                return;
            }

            const b2Rot rotation = b2Body_GetRotation(physicsBody.id);
            const b2Vec2 velocity = b2RotateVector(rotation, b2Vec2{0.0f, move.speed});
            b2Body_SetLinearVelocity(physicsBody.id, velocity);
        });

    // world.view<const CreatureActionReadTouchAreaTag, const CreaturePositionComponent, CreatureSensorTouchComponent,
    //     CreatureStateEnergyComponent>(entt::exclude_t<CreatureOutOfEnergyTag>{}).each(
    //     [&](EcsEntity creature, const CreaturePositionComponent& creaturePosition, CreatureSensorTouchComponent& touchSensor,
    //     CreatureStateEnergyComponent& energy) {
    //         world.erase<CreatureActionReadTouchAreaTag>(creature);
    //         if (!TryConsumeEnergy(world, worldRules, creature, energy, Actions::ReadAreaTouch)) {
    //             return;
    //         }
    //         const auto& [entities] = world.get<const WorldAreaLocatorComponent>(creaturePosition.value);
    //         for (uint8_t i = 0; i < WorldAreaSize; ++i) {
    //             if (const EcsEntity areaPosition = entities[i]; world.any_of<WorldObstacleTag>(areaPosition)) {
    //                 touchSensor.touches[i] = TouchResult::Solid;
    //             } else if (world.any_of<WorldCreatureComponent>(areaPosition)) {
    //                 touchSensor.touches[i] = TouchResult::Elastic;
    //             } else {
    //                 touchSensor.touches[i] = TouchResult::Nothing;
    //             }
    //         }
    //     });

    world.view<const CreatureActionReadEnergyTag, CreatureStateEnergyComponent, CreatureSensorEnergyComponent>(entt::exclude_t<CreatureOutOfEnergyTag>{}).each(
        [&](EcsEntity creature, CreatureStateEnergyComponent &energy, CreatureSensorEnergyComponent &energySensor) {
            world.erase<CreatureActionReadEnergyTag>(creature);
            if (!TryConsumeEnergy(world, worldRules, creature, energy, Actions::ReadCreatureEnergy)) {
                return;
            }
            energySensor.value = energy.value;
        });

    // world.view<const CreatureActionReadRotationTag, const CreatureStateRotationComponent, CreatureSensorRotationComponent,
    //     CreatureStateEnergyComponent>(entt::exclude_t<CreatureOutOfEnergyTag>{}).each(
    //     [&](EcsEntity creature, const CreatureStateRotationComponent &/*rotation*/, CreatureSensorRotationComponent &/*rotationSensor*/,
    //         CreatureStateEnergyComponent &energy) {
    //         world.erase<CreatureActionReadRotationTag>(creature);
    //         if (!TryConsumeEnergy(world, worldRules, creature, energy, Actions::ReadCreatureRotation)) {
    //             return;
    //         }
    //         rotationSensor.value = rotation.value;
    //     });

    // world.view<const CreatureActionBiteComponent, CreatureStateEnergyComponent, const CreatureStateRotationComponent, const
    //     CreaturePositionComponent>(entt::exclude_t<CreatureOutOfEnergyTag>{}).each(
    //     [&](EcsEntity creature, const CreatureActionBiteComponent bite, CreatureStateEnergyComponent& energy, const CreatureStateRotationComponent rotation,
    //     const CreaturePositionComponent position) {
    //         world.erase<CreatureActionBiteComponent>(creature);
    //         if (!TryConsumeEnergy(world, worldRules, creature, energy, Actions::Bite, bite._amplitude)) {
    //             return;
    //         }
    //
    //         const auto& [entities] = world.get<WorldAreaLocatorComponent>(position.value);
    //         if (rotation.value == WorldDirection::InternalCount) {
    //             return;
    //         }
    //
    //         const EcsEntity targetPosition = entities[static_cast<uint8_t>(rotation.value)];
    //         const WorldCreatureComponent* target = world.try_get<const WorldCreatureComponent>(targetPosition);
    //         if (!target) {
    //             return;
    //         }
    //
    //         CreatureStateEnergyComponent& targetEnergy = world.get<CreatureStateEnergyComponent>(target->creature);
    //         const SensorDescription& desc = worldRules.sensorRules[static_cast<uint8_t>(Sensors::CreatureEnergy)];
    //         const uint16_t oldValue = targetEnergy.value;
    //         const SensorCalculationResult r = UpdateSensorValue(desc, targetEnergy.value, -worldRules.consumeEnergyPerBiteMax);
    //         const uint16_t newValue = targetEnergy.value;
    //         const int32_t consumedEnergy = oldValue - newValue;
    //         UpdateSensorValue(desc, energy.value, consumedEnergy);
    //         if (r == SensorCalculationResult::MinValueReached) {
    //             world.emplace_or_replace<CreatureOutOfEnergyTag>(target->creature);
    //         }
    //     });
    {
        const auto creaturesToDestroy = world.view<const CreatureOutOfEnergyTag>();
        // creaturesToDestroy.each([&](const CreaturePositionComponent position) {
        //     // cleanup reference from world to creature
        //     world.erase<WorldCreatureComponent>(position.value);
        // });
        // world.destroy(creaturesToDestroy.begin(), creaturesToDestroy.end());
        world.erase<CreatureOutOfEnergyTag>(creaturesToDestroy.begin(), creaturesToDestroy.end());
    }
}

void FillWorldContent(EcsWorld &world, GameContext &gameContext) {
    std::uniform_real_distribution<float> positionDistX{0.0f, static_cast<float>(gameContext.worldRules.worldSizeX)};
    std::uniform_real_distribution<float> positionDistY{0.0f, static_cast<float>(gameContext.worldRules.worldSizeY)};
    std::uniform_real_distribution<float> radiusDist{1.0f, 10.0f};

    const int32_t initialCellsCount = static_cast<int32_t>(gameContext.worldRules.worldSizeX * gameContext.worldRules.worldSizeY * 0.005f);
    for ([[maybe_unused]] const int32_t _: std::views::iota(0, initialCellsCount)) {
        const EcsEntity creature = world.create();

        const float x = positionDistX(gameContext.randomGenerator);
        const float y = positionDistY(gameContext.randomGenerator);
        const float radius = radiusDist(gameContext.randomGenerator);


        b2BodyDef bodyDef = b2DefaultBodyDef();
        bodyDef.position = {x, y};
        bodyDef.type = b2_kinematicBody;

        const uint64_t rawCreatureId = std::bit_cast<uint32_t>(creature);
        bodyDef.userData = std::bit_cast<void *>(rawCreatureId);

        const b2BodyId bodyId = b2CreateBody(gameContext.physicsWorld, &bodyDef);

        b2ShapeDef shapeDef = b2DefaultShapeDef();
        const b2Circle circle{b2Vec2{0.0f, 0.0f}, radius};
        b2CreateCircleShape(bodyId, &shapeDef, &circle);

        world.emplace<WorldCreatureTag>(creature);
        world.emplace<PhysicsBodyComponent>(creature, bodyId);
        InitCreatureGenome(world, creature);
    }
}

void SetupWorldRules(const int32_t &ScreenWidth, const int32_t &ScreenHeight, GameContext &gameContext) {
    WorldDescription &worldRules = gameContext.worldRules;
    {
        static constexpr std::string_view RandomSeed = "White";
        std::seed_seq seed{RandomSeed.begin(), RandomSeed.end()};
        gameContext.randomGenerator.seed(seed);
    }

    static constexpr int32_t WorldSize = 1000;
    gameContext.worldLocator = WorldLocator(WorldSize, WorldSize);
    gameContext.camera.SetScreenSize(ScreenWidth, ScreenHeight);

    static constexpr int32_t BrainInputCount = GenesCount + SensorsCount;
    static constexpr float SigmoidMeaningfulRangeSize = 10.0f; //< [-5; 5]
    worldRules.neuronValueRangeSize = static_cast<float>(BrainInputCount) / SigmoidMeaningfulRangeSize;

    worldRules.worldSizeX = WorldSize;
    worldRules.worldSizeY = WorldSize;
    worldRules.creatureEnergyMax = 100000;
    worldRules.brainRestPerTickMin = 5;
    worldRules.creatureFatigueMax = 200;

    auto MakeGene = [&](Genes gene, std::string_view name, float mutationSigma) {
        worldRules.geneRules[static_cast<uint8_t>(gene)] = {name, 0, AvailabilityResultCount, 0.0f, mutationSigma};
    };

    // worldRules.actionRules[static_cast<uint8_t>(Actions::ReadCreatureRotation)] = {"Read Creature Rotation", 5, 0};
    // worldRules.actionRules[static_cast<uint8_t>(Actions::ReadAreaTouch)] = {"Read Area Touch", 15, 10};
    // worldRules.actionRules[static_cast<uint8_t>(Actions::Bite)] = {"Bite", 50, 5};
    // worldRules.actionRules[static_cast<uint8_t>(Actions::Rotate)] = {"Rotate", 10, 5};
    // worldRules.actionRules[static_cast<uint8_t>(Actions::MakeChild)] = {"Make Child", 200, 15};

    MakeGene(Genes::GeneCreatureEnergySensor, "Energy Sensor", 0.01f);
    MakeGene(Genes::GeneCreatureRotationSensor, "Rotation Sensor", 0.01f);
    MakeGene(Genes::GeneCreatureFatigueSensor, "Fatigue Sensor", 0.01f);
    MakeGene(Genes::GeneTouchAreaSensor, "Touch Area Sensor", 0.01f);
    MakeGene(Genes::GeneCanMove, "Can Move", 0.01f);
    MakeGene(Genes::GeneCanBite, "Can Bite", 0.01f);
    MakeGene(Genes::GeneCanRotate, "Can Rotate", 0.01f);
    MakeGene(Genes::GeneCanMakeChild, "Can Make Child", 0.01f);
    MakeGene(Genes::GeneCreatureMutationSpeedFactor, "Mutation Speed", 0.01f); //< todo: remove?

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

    auto MakeSigmoidAction = [&](Actions action, ActionDescription desc, SigmoidActionType sigmoid) {
        const auto index = static_cast<uint8_t>(action);
        ASSERT(!worldRules.linearActions[index].has_value() && !worldRules.sigmoidActions[index].has_value(), "sanity check: action has been registered already!");
        worldRules.sigmoidActions[index] = std::move(sigmoid);
        worldRules.actionRules[index] = std::move(desc);
    };
    auto MakeLinearAction = [&](Actions action, ActionDescription desc, LinearActionType linear) {
        const auto index = static_cast<uint8_t>(action);
        ASSERT(!worldRules.linearActions[index].has_value() && !worldRules.sigmoidActions[index].has_value(), "sanity check: action has been registered already!");
        worldRules.linearActions[index] = std::move(linear);
        worldRules.actionRules[index] = std::move(desc);
    };

    MakeSigmoidAction(Actions::MoveIntent, ActionDescription{"Move Intent", 15, 5}, SigmoidActionType{0.5f});
    MakeLinearAction(Actions::MoveVelocity, ActionDescription{"Velocity", 5, 1}, LinearActionType{0.0f, 1.0f, true});

    MakeLinearAction(Actions::ReadCreatureEnergy, ActionDescription{"Move Intent", 15, 5}, LinearActionType{0.5f,});

    worldRules.actionRules[static_cast<uint8_t>(Actions::ReadCreatureEnergy)] = {"Read Creature Energy", 5, 0};
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
    const auto [cameraPositionX, cameraPositionY] = context.camera.GetPosition();

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
    ImGui::Text("Mouse: position = (%.0f, %.0f), scroll = %.2f", mousePosX, mousePosY, mouseWheelDelta);
    ImGui::Text("Camera: position = (%.0f, %.0f), zoom = %.2f", cameraPositionX, cameraPositionY, context.camera.GetZoom());
    const b2Counters counters = b2World_GetCounters(context.physicsWorld);
    ImGui::Text("Physics body: %d", counters.bodyCount);
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
    {
        const b2WorldDef worldDef = b2DefaultWorldDef();
        gameContext.physicsWorld = b2CreateWorld(&worldDef);
    }
    SetupWorldRules(ScreenWidth, ScreenHeight, gameContext);
    FillWorldContent(world, gameContext);

    /// Main loop
    bool shouldStopMainLoop = false;
    std::chrono::steady_clock::time_point lastFrameTime = std::chrono::steady_clock::now();

    std::chrono::steady_clock::duration physicsRestTime;
    static constexpr std::chrono::steady_clock::duration PhysicsStep = std::chrono::milliseconds(30);

    while (!shouldStopMainLoop) {
        const std::chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();
        const std::chrono::steady_clock::duration elapsedFrameTime = currentTime - lastFrameTime;
        lastFrameTime = currentTime;

        SDL_Event event{};
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL3_ProcessEvent(&event);
            const bool stopByQuitEvent = event.type == SDL_EVENT_QUIT;
            const bool stopByWindowEvent = event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED && event.window.windowID == SDL_GetWindowID(window);
            shouldStopMainLoop |= stopByQuitEvent || stopByWindowEvent;
        }

        physicsRestTime += elapsedFrameTime;
        if (physicsRestTime > PhysicsStep) {
            const int64_t physicsSteps = physicsRestTime / PhysicsStep;
            for (int i = 0; i < physicsSteps; ++i) {
                static constexpr float PhysicsStepSeconds = 1.0f / std::chrono::duration_cast<std::chrono::milliseconds>(PhysicsStep).count();
                b2World_Step(gameContext.physicsWorld, PhysicsStepSeconds, 4);
            }
            physicsRestTime -= physicsSteps * PhysicsStep;
        }

        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();
        ProcessImGui(std::chrono::duration_cast<std::chrono::milliseconds>(elapsedFrameTime), world);
        ImGui::Render();

        ProcessWorldUpdate(world);

        SDL_SetRenderDrawColor(renderer, 0x70, 0x70, 0x70, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);
        rasterizationTarget.Lock();
        {
            const auto [centerX, centerY] = gameContext.camera.GetPosition();
            const auto [sizeX, sizeY] = gameContext.camera.GetSize();
            const b2AABB visibleRegion{b2Vec2{centerX - sizeX / 2.0f, centerY - sizeY / 2.0f}, b2Vec2{centerX + sizeX / 2.0f, centerY + sizeY / 2.0f}};

            const b2QueryFilter queryFilter = b2DefaultQueryFilter();
            auto overlapResultFcn = [](b2ShapeId shapeId, void *userData) -> bool {
                EcsWorld &world = *static_cast<EcsWorld *>(userData);
                const b2BodyId bodyId = b2Shape_GetBody(shapeId);
                void *bodyUserData = b2Body_GetUserData(bodyId);
                if (!bodyUserData) {
                    return true;
                }

                const uint32_t rawCreatureId = static_cast<uint32_t>(std::bit_cast<uint64_t>(bodyUserData));
                const EcsEntity entity = std::bit_cast<EcsEntity>(rawCreatureId);

                world.emplace<RenderDirtyTag>(entity);
                return true;
            };

            b2World_OverlapAABB(gameContext.physicsWorld, visibleRegion, queryFilter, overlapResultFcn, &world);
            auto entitiesToRender = world.view<const RenderDirtyTag, const PhysicsBodyComponent>();
            const auto debugCount = entitiesToRender.size_hint();
            entitiesToRender.each([&](EcsEntity entity, const PhysicsBodyComponent physicsBody) {
                world.erase<RenderDirtyTag>(entity);

                const b2Vec2 position = b2Body_GetPosition(physicsBody.id);
                b2ShapeId shapeId{};
                const int32_t shapesCount = b2Body_GetShapes(physicsBody.id, &shapeId, 1);
                ASSERT(shapesCount == 1, "sanity check: body should have only one shape!");
                const b2Circle circle = b2Shape_GetCircle(shapeId);
                const float creatureScreenSpaceRadius = circle.radius * gameContext.camera.GetZoom();

                const auto [screenSpaceX, screenSpaceY] = gameContext.camera.ToScreenSpace(position.x, position.y);
                rasterizationTarget.SetFilledCircle(screenSpaceX, screenSpaceY, creatureScreenSpaceRadius, SDL_Color{0, 200, 0, SDL_ALPHA_OPAQUE});
            });

            {
                const auto unusedTags = world.view<const RenderDirtyTag>();
                world.erase<RenderDirtyTag>(unusedTags.begin(), unusedTags.end());
            }
        }
        rasterizationTarget.Unlock();
        if (!SDL_RenderTexture(renderer, renderTargetTexture, nullptr, nullptr)) {
            ASSERT_FAIL("Sanity check: failed to render texture");
            break;
        }
        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);
        SDL_RenderPresent(renderer);
    }

    b2DestroyWorld(gameContext.physicsWorld);

    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyTexture(renderTargetTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
