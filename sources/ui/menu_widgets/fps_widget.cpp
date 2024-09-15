#include "fps_widget.h"

BaseMenuWidget::MenuWidgetAction FpsWidget::ProcessMenuItem(Common::Time elapsedTime)
{
    bool opened = true;
    if (ImGui::Begin("FPS", &opened)) {
        _frameTimeCounter.AddSample(elapsedTime.AsMilliseconds());
        const int32_t frameDuration = _frameTimeCounter.CalcMedian();
        const int32_t fps = static_cast<int32_t>(std::round(1000.0f / static_cast<float>(frameDuration)));
        ImGui::Text("FPS  %5d", fps);
        ImGui::Text("Time %5d [ms]", frameDuration);
    }
    ImGui::End();
    return opened ? MenuWidgetAction::KeepOpen : MenuWidgetAction::ShouldClose;
}