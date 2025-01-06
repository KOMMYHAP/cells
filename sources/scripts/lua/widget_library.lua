---@class ui
---@field created_widgets table<string, menu_widget>
ui = {
    created_widgets={}
}

---@param parent menu_widget?
---@param name string
---@return menu_widget widget
function ui:require_widget(parent, name)
    local widget = ui.created_widgets[name]
    if widget ~= nil then
        return widget
    end

    local parent_id = ui_native.root_widget
    if parent then
        parent_id = parent.id
    end
    local w = ui_native:register(parent_id, name)
    if not w then
        error("Failed to create widget "..name)
    end

    ui.created_widgets[name] = w
    return w
end

---@param parent menu_widget?
---@param name string
---@return menu_widget widget
function ui:require_group_widget(parent, name)
    local w = ui:require_widget(parent, name)
    w.onUpdate = function ()
        return true
    end
    return w
end