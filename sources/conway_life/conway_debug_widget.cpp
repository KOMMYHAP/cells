#include "conway_debug_widget.h"

#include "conway_game.h"
#include "conway_game_system.h"
#include "imgui.h"

ConwayDebugWidget::ConwayDebugWidget(const UiConfig& uiConfig, ConwayGameSystem& game)
    : _game(&game)
    , _uiConfig(uiConfig)
{
}

BaseMenuWidget::MenuWidgetAction ConwayDebugWidget::ProcessMenuItem(Common::Time /*time*/)
{
    ProcessGameSummary();
    ProcessFieldEditor();
    const auto editorStateStr = [](EditorState state) -> std::string_view {
        switch (state) {
        case EditorState::None:
            return "none";
        case EditorState::Drawing:
            return "drawing";
        case EditorState::Erasing:
            return "erasing";
        default:
            return "unknown";
        }
    }(_editorState);

    ImGui::Text("Editor state: %.*s", static_cast<int>(editorStateStr.size()), editorStateStr.data());

    return MenuWidgetAction::KeepOpen;
}

void ConwayDebugWidget::ProcessFieldEditor()
{
    const bool leftButtonDragging = ImGui::IsMouseDragging(ImGuiMouseButton_Left);
    const bool rightButtonDragging = ImGui::IsMouseDragging(ImGuiMouseButton_Right);
    if (!leftButtonDragging && !rightButtonDragging) {
        UpdateEditorState(EditorState::None);
        return;
    }

    if (leftButtonDragging) {
        UpdateEditorState(EditorState::Drawing);
    } else {
        UpdateEditorState(EditorState::Erasing);
    }

    if (!_game->ModifyGame()) {
        return;
    }

    const ImVec2 position = ImGui::GetMousePos();
    auto fieldPosition = ConvertMouseToFieldPosition(position.x, position.y);
    if (!fieldPosition) {
        return;
    }
    Field& field = _game->ModifyGame()->ModifyCurrentGeneration();
    field.Set(*fieldPosition, _editorState == EditorState::Drawing);
}

void ConwayDebugWidget::UpdateEditorState(EditorState newState)
{
    if (_editorState == newState) {
        return;
    }

    _editorState = newState;
}

std::optional<FieldPosition> ConwayDebugWidget::ConvertMouseToFieldPosition(float x, float y)
{
    if (x < _uiConfig.worldWidgetOffsetX || y < _uiConfig.worldWidgetOffsetY) {
        return {};
    }
    if (x >= _uiConfig.worldWidgetOffsetX + _uiConfig.worldWidgetSizeX || y >= _uiConfig.worldWidgetOffsetY + +_uiConfig.worldWidgetSizeY) {
        return {};
    }

    const int32_t fieldX = static_cast<int32_t>((x - _uiConfig.worldWidgetOffsetX) / _uiConfig.cellPixelsSize);
    const int32_t fieldY = static_cast<int32_t>((y - _uiConfig.worldWidgetOffsetY) / _uiConfig.cellPixelsSize);
    return FieldPosition { fieldX, fieldY };
}

void ConwayDebugWidget::ProcessGameSummary()
{
    const ConwayGameSummary& gameSummary = _game->GetGameSummary();
    const std::string_view stateStr = [](ConwayGenerationState state) -> std::string_view {
        switch (state) {
        case ConwayGenerationState::Good:
            return "game in progress";
        case ConwayGenerationState::NoCellsLeft:
            return "no cells left";
        case ConwayGenerationState::SameField:
            return "same configuration";
        default:
            return "<unknown>";
        };
    }(gameSummary.state);

    ImGui::Text("State: %.*s", static_cast<int32_t>(stateStr.length()), stateStr.data());
    ImGui::Text("Generation: %d", _game->GetGame()->GetCurrentGenerationNumber());
    ImGui::Text("Cells: %d", gameSummary.aliveCells);
    if (gameSummary.similarGenerationNumber) {
        ImGui::Text("Similar generation: %d", *gameSummary.similarGenerationNumber);
    }

    if (gameSummary.state != ConwayGenerationState::Good) {
        if (ImGui::Button("Restart")) {
            _game->Restart();
        }
    }
}