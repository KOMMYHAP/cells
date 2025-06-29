require "ui.widget_library"

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

TOTAL_SAMPLES_COUNT = 50
for i=1, TOTAL_SAMPLES_COUNT do
    statistics.samples[i] = 0
end
statistics.sample_position = 1

statistics.widget.onUpdate = function (dt)
    statistics.samples[statistics.sample_position] = dt
    statistics.sample_position = statistics.sample_position + 1
    if statistics.sample_position > #statistics.samples then
        statistics.sample_position = 1
    end

    local total_ms = 0.0
    for i=1, #statistics.samples do
        total_ms = total_ms + statistics.samples[i]
    end
    average_ms = total_ms / TOTAL_SAMPLES_COUNT
    local average_fps = 1000.0 / average_ms

    ImGui.Text(string.format("FPS: %.2f (%.2f ms)", average_fps, average_ms))
    local gc_memory_usage = lua_native:get_memory_used() // 1024
    local gc_is_on = lua_native:is_gc_on() and "on" or "off"
    ImGui.Text(string.format("Lua Memory: %d KiB (gc is %s)", gc_memory_usage, gc_is_on))

    return true
end

return engine_group
