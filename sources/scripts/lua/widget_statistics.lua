require "widget_library"

local engine_group = ui:require_group_widget(nil, "Engine")

---@class statistics_widget
---@field widget menu_widget
---@field samples table<integer, number>
---@field sample_position number
local statistics = {
    widget = ui:require_widget(engine_group, "Statistics"),
    sample_position = 1,
    samples = {}
}

for i=1, 30 do
    statistics.samples[i] = 0
end
statistics.sample_position = 1

statistics.widget.onUpdate = function (dt)
    statistics.samples[statistics.sample_position] = dt
    statistics.sample_position = statistics.sample_position + 1
    if statistics.sample_position > #statistics.samples then
        statistics.sample_position = 1
    end

    local average_ms = 0.0
    for i=1, #statistics.samples do
        average_ms = average_ms + statistics.samples[i]
    end
    average_ms = average_ms / #statistics.samples
    local average_fps = 1000.0 / average_ms

    ImGui.Text(string.format("FPS: %.3f (%.3f ms)", average_fps, average_ms))

    return true
end

return engine_group
