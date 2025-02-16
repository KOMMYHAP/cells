require "ui.widget_library"

local demo_group = ui:require_group_widget(nil, "Demo")
local imgui_demo = ui:require_widget(demo_group, "ImGui")
imgui_demo.onUpdate = function ()
    return ImGui.ShowDemo()
end

local implot_demo = ui:require_widget(demo_group, "ImPlot")
implot_demo.onUpdate = function ()
    return ImPlot.ShowDemo()
end

return demo_group
