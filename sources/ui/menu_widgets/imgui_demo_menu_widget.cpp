#include "imgui_demo_menu_widget.h"

BaseMenuWidget::MenuWidgetAction ImGuiDemoMenuWidget::ProcessMenuItem(Common::Time /*elapsedTime*/)
{
    bool opened = true;
    ImGui::ShowDemoWindow(&opened);
    return opened ? MenuWidgetAction::KeepOpen : MenuWidgetAction::ShouldClose;
}