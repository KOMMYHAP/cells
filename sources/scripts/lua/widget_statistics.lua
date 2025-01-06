require "widget_library"

local engine_group = ui_menu:require_group_widget(nil, "Engine")

---@class statistics_widget: menu_widget
---@field samples table
---@field sample_position number
local statistics = ui_menu:require_widget(engine_group, "Statistics")

statistics.samples = {}
for i=1, 30 do
    statistics.samples[i] = 0
end
statistics.sample_position = 1

statistics.onUpdate = function (dt)
    statistics.samples[statistics.sample_position] = dt
    statistics.sample_position = statistics.sample_position + 1
    if statistics.sample_position > #statistics.samples then
        statistics.sample_position = 1
    end

    local average_ms = 0.0
    for i=1, #statistics.samples do
        average_ms = average_ms + statistics.samples[i]
    end

    local average_fps = 1000.0 / average_ms

    ImGui.Text(string.format("FPS: %03d (%03d ms)", average_ms, average_fps))

    return true
end

return engine_group
