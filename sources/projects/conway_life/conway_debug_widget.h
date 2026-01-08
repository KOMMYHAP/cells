#pragma once
#include "components/generated/auto_cell_position.h"
#include "menu_widgets/base/base_menu_widget.h"
#include "simulation/common_adapter.h"
#include "simulation/simulation_ecs_config.h"

class CellLocator;
struct UiConfig;
class ConwayGameController;
struct ConwayGameSummary;

class ConwayDebugWidget final : public BaseMenuWidget {
public:
    ConwayDebugWidget(SimulationStorage& storage);

    MenuWidgetAction ProcessMenuItem(Common::Time) override;

private:
    enum class EditorState {
        None,
        Drawing,
        Erasing,
    };

    void ProcessFieldEditor();
    void UpdateEditorState(EditorState newState);
    std::optional<CellPosition> ConvertMouseToFieldPosition(float x, float y);

    void ProcessGameSummary();

    ConwayGameController* _game { nullptr };
    const UiConfig* _uiConfig{nullptr};
    EditorState _editorState {EditorState::None};
    EcsWorld* _ecsWorld{nullptr};
    const CellLocator* _cellLocator{nullptr};

};