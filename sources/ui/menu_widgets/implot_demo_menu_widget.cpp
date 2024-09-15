#include "implot_demo_menu_widget.h"

BaseMenuWidget::MenuWidgetAction ImPlotDemoMenuWidget::ProcessMenuItem(Common::Time elapsedTime)
{
    bool opened = true;
    ImPlot::ShowDemoWindow(&opened);
    return opened ? MenuWidgetAction::KeepOpen : MenuWidgetAction::ShouldClose;
}