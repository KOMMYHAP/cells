#include "conway_debug_widget.h"

#include "components/generated/auto_has_life_tag.h"
#include "components/generated/auto_will_born_tag.h"
#include "components/generated/auto_will_die_tag.h"
#include "conway_game.h"
#include "imgui.h"
#include "simulation/cell_locator.h"
#include "ui_config.h"

ConwayDebugWidget::ConwayDebugWidget(SimulationStorage& storage)
    : _game(&storage.Modify<ConwayGameController>())
    , _uiConfig(&storage.Get<UiConfig>())
    , _ecsWorld(&storage.Modify<EcsWorld>())
    , _cellLocator(&storage.Get<CellLocator>())
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
    const bool mouseOverWindow = ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow);
    const bool leftButtonDragging = ImGui::IsMouseDragging(ImGuiMouseButton_Left);
    const bool rightButtonDragging = ImGui::IsMouseDragging(ImGuiMouseButton_Right);
    const bool shouldStopEditing = mouseOverWindow || (!leftButtonDragging && !rightButtonDragging);
    if (shouldStopEditing) {
        UpdateEditorState(EditorState::None);
        return;
    }

    if (leftButtonDragging) {
        UpdateEditorState(EditorState::Drawing);
    } else {
        UpdateEditorState(EditorState::Erasing);
    }

    const ImVec2 position = ImGui::GetMousePos();
    auto fieldPosition = ConvertMouseToFieldPosition(position.x, position.y);
    if (!fieldPosition) {
        return;
    }

    const EcsEntity cell = _cellLocator->Find(*fieldPosition);
    if (cell == InvalidEcsEntity) {
        return;
    }

    const bool hasLife = _ecsWorld->all_of<HasLifeTag>(cell);
    if (_editorState == EditorState::Drawing && !hasLife) {
        _ecsWorld->emplace_or_replace<WillBornTag>(cell);
    } else if (_editorState == EditorState::Erasing && hasLife) {
        _ecsWorld->emplace_or_replace<WillDieTag>(cell);
    }
}

void ConwayDebugWidget::UpdateEditorState(EditorState newState)
{
    if (_editorState == newState) {
        return;
    }

    _editorState = newState;
}

std::optional<CellPosition> ConwayDebugWidget::ConvertMouseToFieldPosition(float x, float y)
{
    if (x < _uiConfig->worldWidgetOffsetX || y < _uiConfig->worldWidgetOffsetY) {
        return {};
    }
    if (x >= _uiConfig->worldWidgetOffsetX + _uiConfig->worldWidgetSizeX || y >= _uiConfig->worldWidgetOffsetY + +_uiConfig->worldWidgetSizeY) {
        return {};
    }

    const int16_t fieldX = static_cast<int16_t>((x - _uiConfig->worldWidgetOffsetX) / _uiConfig->cellPixelsSize);
    const int16_t fieldY = static_cast<int16_t>((y - _uiConfig->worldWidgetOffsetY) / _uiConfig->cellPixelsSize);
    return CellPosition { fieldX, fieldY };
}

void ConwayDebugWidget::ProcessGameSummary()
{
    const ConwayGameSummary& gameSummary = _game->GetSummary();
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
    ImGui::Text("Generation: %d", gameSummary.generationNumber);
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