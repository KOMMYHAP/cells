#include "engine_summary_widget.h"

#include "system/app_statistics.h"

EngineSummaryWidget::EngineSummaryWidget(const AppStatistics& appStats)
    : _appStats(&appStats)
{
}

BaseMenuWidget::MenuWidgetAction EngineSummaryWidget::ProcessMenuItem(Common::Time /*time*/)
{
    ImGui::Text("Frame: %" PRId64, _appStats->GetElapsedFramesCount());
    const Common::Time frameTime = _appStats->GetMedianFrameTime();
    ImGui::Text("FPS: %.2f (%" PRId64 " ms)", 1.0f / frameTime.AsSeconds(), frameTime.AsMilliseconds());
    return MenuWidgetAction::KeepOpen;
}