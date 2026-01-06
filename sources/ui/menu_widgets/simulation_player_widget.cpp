#include "simulation_player_widget.h"

#include "simulation/simulation_player.h"

SimulationPlayerWidget::SimulationPlayerWidget(SimulationPlayer& player)
    : _player(&player)
{
}

BaseMenuWidget::MenuWidgetAction SimulationPlayerWidget::ProcessMenuItem(Common::Time /*time*/)
{
    const std::string_view modeStr = [](SimulationPlayer::Mode mode) -> std::string_view {
        switch (mode) {
        case SimulationPlayer::Mode::ManualStep:
            return "Manual Step";
        case SimulationPlayer::Mode::FixedSpeed:
            return "Fixed Speed";
        default:
            return "<unknown>";
        }
    }(_player->GetCurrentMode());

    ImGui::Text("Current mode: %.*s", static_cast<int>(modeStr.length()), modeStr.data());

    ImGui::Text("Generations per second:");
    ImGui::SameLine();
    const bool speedChanged = ImGui::DragInt("##GenerationsPerSecond", &_generationsPerSecond);
    _generationsPerSecond = std::clamp(_generationsPerSecond, 1, GenerationsPerSecondLimit);
    if (speedChanged) {
        _player->SetFixedSpeedMode(_generationsPerSecond);
    }

    if (_player->GetCurrentMode() == SimulationPlayer::Mode::ManualStep) {
        if (ImGui::Button("Next Frame")) {
            _player->RequestStep();
        }
    } else {
        if (ImGui::Button("Pause")) {
            _player->Pause();
        }
    }

    return MenuWidgetAction::KeepOpen;
}