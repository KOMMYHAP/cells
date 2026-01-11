#include "game_widget.h"

#include "components/generated/auto_emitter_init_request.h"
#include "game_controller.h"
#include "imgui.h"

GameWidget::GameWidget(EcsWorld& ecsWorld, GameController& gameController, GameConfig& gameConfig)
    : _ecsWorld(&ecsWorld)
    , _gameController(&gameController)
    , _gameConfig(&gameConfig)
{
}

BaseMenuWidget::MenuWidgetAction GameWidget::ProcessMenuItem(Common::Time /*time*/)
{
    const int64_t frameTimeMs = _gameController->GetFrameTime().AsMilliseconds();
    const double fps = 1000.0 / frameTimeMs;
    ImGui::Text("%.2f/%" PRId64 " (fps, ms)", fps, frameTimeMs);

    ImGui::Text("%d/%d (emitters/particles)", _gameController->GetEmittersCount(), _gameController->GetParticlesCount());

    ProcessGravity(_gameController->ModifyGravityUnit());
    ProcessEmitters();
    ProcessGenerations();

    return MenuWidgetAction::KeepOpen;
}

void GameWidget::ProcessEmitters()
{
    if (ImGui::TreeNode("Emitters")) {
        for (int32_t i = 0; i < _emitters.size(); ++i) {
            EmitterData& emitterData = _emitters[i];
            ProcessEmitterEntry(emitterData);
            if (i + 1 != _emitters.size()) {
                ImGui::Separator();
            }
        }
        if (ImGui::Button("Create emitter")) {
            _emitters.emplace_back();
        }
        ImGui::TreePop();
    }

    for (EmitterData& data : _emitters) {
        if (!data.shouldRemove && !data.shouldRecreate) {
            continue;
        }

        if (data.emitter != InvalidEcsEntity) {
            _ecsWorld->destroy(data.emitter);
        }

        if (!data.shouldRecreate) {
            continue;
        }

        data.emitter = _ecsWorld->create();
        _ecsWorld->emplace<EmitterInitRequest>(data.emitter, data.generation, data.position[0], data.position[1]);
        data.shouldRecreate = false;
    }

    auto removeRange = std::ranges::remove_if(_emitters, [](const EmitterData& emitterData) {
        return emitterData.shouldRemove;
    });
    _emitters.erase(removeRange.begin(), removeRange.end());
}

void GameWidget::ProcessEmitterEntry(EmitterData& emitterData)
{
    if (emitterData.emitter != InvalidEcsEntity && !_ecsWorld->valid(emitterData.emitter)) {
        emitterData.emitter = InvalidEcsEntity;
    }

    if (emitterData.emitter != InvalidEcsEntity) {
        ImGui::LabelText("Id", "%d", emitterData.emitter);
    } else {
        ImGui::LabelText("Id", "%s", "Invalid");
    }

    ImGui::DragFloat2("Position", emitterData.position);
    ImGui::DragInt("Generation", &emitterData.generation, 1.0f, 0, static_cast<int32_t>(_generations.size()), "%d", ImGuiSliderFlags_AlwaysClamp);
    ImGui::Checkbox("Remove", &emitterData.shouldRemove);
    ImGui::SameLine();
    ImGui::Checkbox("Recreate", &emitterData.shouldRecreate);
}

void GameWidget::ProcessGenerations()
{
    if (ImGui::TreeNode("Generations")) {
        for (int32_t i = 0; i < _generations.size(); ++i) {
            GenerationData& generationData = _generations[i];
            ProcessGenerationEntry(generationData);
            if (i + 1 != _generations.size()) {
                ImGui::Separator();
            }
        }
        if (ImGui::Button("Create generation")) {
            _generations.emplace_back();
        }
        ImGui::TreePop();
    }

    auto removeRange = std::ranges::remove_if(_generations, [](const GenerationData& generationData) {
        return generationData.shouldRemove;
    });
    _generations.erase(removeRange.begin(), removeRange.end());

    std::vector<GameConfig::FireworksConfig>& fireworks = _gameConfig->ModifyFireworks();
    fireworks.clear();
    for (GenerationData& generationData : _generations) {
        fireworks.emplace_back(generationData.config);
    }
}

void GameWidget::ProcessGenerationEntry(GenerationData& generationData)
{
    GameConfig::FireworksConfig& config = generationData.config;
    float directionAngle[2] {
        config.minAngle,
        config.maxAngle
    };
    float speed[2] = { config.minSpeed, config.maxSpeed };

    {
        ImGui::Text("Emitter:");
        ImGui::PushID("Emitters");
        ImGui::DragInt("Frames to live", &config.emitterFramesToLive, 1.0f, 1, INT_MAX, "%d", ImGuiSliderFlags_AlwaysClamp);
        ImGui::DragFloat("Particles/sec", &config.emitterParticlesPerSeconds, 1.0f, 0.0f, FLT_MAX, "%.1f", ImGuiSliderFlags_AlwaysClamp);
        ImGui::PopID();
    }
    {
        ImGui::Text("Particle:");
        ImGui::PushID("Particles");
        ImGui::DragFloat2("Angle (min/max)", directionAngle, 5.0f, -180.0f, 180.0f, "%.3f", ImGuiSliderFlags_AlwaysClamp);
        ImGui::DragFloat2("Speed (min/max)", speed, 10.0f, 0.0f, 1000.0f, "%.3f", ImGuiSliderFlags_AlwaysClamp);
        ImGui::DragInt("Frames to live", &config.framesToLive, 1.0f, 1, INT_MAX, "%d", ImGuiSliderFlags_AlwaysClamp);
        ImGui::PopID();
    }

    config.minAngle = directionAngle[0];
    config.maxAngle = directionAngle[1];
    config.minSpeed = speed[0];
    config.maxSpeed = speed[1];
}

void GameWidget::ProcessGravity(GravityUnit& gravity)
{
    ImGui::Text("Gravity:");

    bool gravityEnabled = _gameController->IsGravityEnabled();
    ImGui::Checkbox("Enabled", &gravityEnabled);
    _gameController->SetGravityEnabled(gravityEnabled);

    float gravityCenterXY[2] {
        static_cast<float>(gravity.positionX),
        static_cast<float>(gravity.positionY)
    };
    ImGui::DragFloat2("Center", gravityCenterXY);
    gravity.positionX = gravityCenterXY[0];
    gravity.positionY = gravityCenterXY[1];

    double constantStep = 1e-10;
    double constantStepFast = 1e-9;
    ImGui::InputScalar("Constant", ImGuiDataType_Double, &gravity.constant, &constantStep, &constantStepFast, "%e");

    double gravityStep = 1e24;
    double gravityStepFast = 1e25;
    ImGui::InputScalar("Mass", ImGuiDataType_Double, &gravity.mass, &gravityStep, &gravityStepFast, "%e");
}