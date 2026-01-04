#pragma once
#include "field_position.h"
#include "menu_widgets/base/base_menu_widget.h"
#include "ui_config.h"

class ConwayGameSystem;
class ConwayGame;
struct ConwayGameSummary;

class ConwayDebugWidget final : public BaseMenuWidget {
public:
    ConwayDebugWidget(const UiConfig& uiConfig, ConwayGameSystem& game);

    MenuWidgetAction ProcessMenuItem(Common::Time) override;

private:
    enum class EditorState {
        None,
        Drawing,
        Erasing,
    };

    void ProcessFieldEditor();
    void UpdateEditorState(EditorState newState);
    std::optional<FieldPosition> ConvertMouseToFieldPosition(float x, float y);

    void ProcessGameSummary();

    ConwayGameSystem* _game { nullptr };
    UiConfig _uiConfig;
    EditorState _editorState {EditorState::None};

};