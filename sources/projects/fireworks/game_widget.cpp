#include "game_widget.h"

#include "game_controller.h"
#include "imgui.h"

GameWidget::GameWidget(GameController& gameController, GameConfig& gameConfig)
    : _gameController(&gameController)
    , _gameConfig(&gameConfig)
{
}

BaseMenuWidget::MenuWidgetAction GameWidget::ProcessMenuItem(Common::Time /*time*/)
{
    const int64_t frameTimeMs = _gameController->GetFrameTime().AsMilliseconds();
    const double fps = 1000.0 / frameTimeMs;
    ImGui::Text("%.2f/%" PRId64 " (fps, ms)", fps, frameTimeMs);

    ImGui::Text("%d/%d (emitters/particles)", _gameController->GetEmittersCount(), _gameController->GetParticlesCount());
    bool gravityEnabled = _gameController->IsGravityEnabled();

    ImGui::Text("Gravity:");
    ImGui::Checkbox("Enabled", &gravityEnabled);
    _gameController->SetGravityEnabled(gravityEnabled);
    const CellPosition gravityCenter = _gameController->GetGravityCenter();
    float gravityCenterXY[2] {
        static_cast<float>(gravityCenter.x),
        static_cast<float>(gravityCenter.y)
    };
    ImGui::DragFloat2("Center", gravityCenterXY);
    _gameController->SetGravityCenter(CellPosition { static_cast<int16_t>(gravityCenterXY[0]), static_cast<int16_t>(gravityCenterXY[1]) });
    double gravityConstant = _gameController->GetGravityConstant();
    double gravityMin = std::numeric_limits<double>::min();
    double gravityMax = std::numeric_limits<double>::max();
    ImGui::DragScalar("Constant", ImGuiDataType_Double, &gravityConstant, 100000.0f, &gravityMin, &gravityMax, "%f");
    _gameController->SetGravityConstant(gravityConstant);

    ProcessFireworksConfig(_gameConfig->ModifyFireworks()[0]);

    return MenuWidgetAction::KeepOpen;
}

void GameWidget::ProcessFireworksConfig(GameConfig::FireworksConfig& config)
{
    float directionX[2] = { config.minDirectionX, config.maxDirectionX };
    float directionY[2] = { config.minDirectionY, config.maxDirectionY };
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
        ImGui::DragFloat2("Direction X (min/max)", directionX, 0.02f, -1.0f, 1.0f, "%.3f", ImGuiSliderFlags_AlwaysClamp);
        ImGui::DragFloat2("Direction Y (min/max)", directionY, 0.02f, -1.0f, 1.0f, "%.3f", ImGuiSliderFlags_AlwaysClamp);
        ImGui::DragFloat2("Speed (min/max)", speed, 10.0f, 0.0f, 1000.0f, "%.3f", ImGuiSliderFlags_AlwaysClamp);
        ImGui::DragInt("Frames to live", &config.framesToLive, 1.0f, 1, INT_MAX, "%d", ImGuiSliderFlags_AlwaysClamp);
        ImGui::PopID();
    }

    config.minDirectionX = directionX[0];
    config.maxDirectionX = directionX[1];
    config.minDirectionY = directionY[0];
    config.maxDirectionY = directionY[1];
    config.minSpeed = speed[0];
    config.maxSpeed = speed[1];
}