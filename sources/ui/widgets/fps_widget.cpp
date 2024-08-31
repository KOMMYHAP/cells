#include "fps_widget.h"

void FpsWidget::UpdateWidget(sf::Time elapsedTime)
{
    _frameTimeCounter.AddSample(elapsedTime.asMilliseconds());
    const int32_t frameDuration = _frameTimeCounter.CalcMedian();
    const int32_t fps = static_cast<int32_t>(std::round(1.0f / static_cast<float>(frameDuration)));
    ImGui::Text("FPS %3d", fps);
    ImGui::Text("Time %5d [ms]", frameDuration);
}