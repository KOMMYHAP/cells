---@param parent menu_widget?
---@param name string
---@return menu_widget widget
function ui_menu:require_widget(parent, name)
    local widget = ui_menu[name]
    if widget ~= nil then
        return widget
    end

    local parent_id = ui_menu.root_widget
    if parent then
        parent_id = parent.id 
    end
    local w = ui_menu:register(parent_id, name)
    if not w then
        error("Failed to create widget "..name)
    end

    ui_menu[name] = w
    return w
end

---@param parent menu_widget?
---@param name string
---@return menu_widget widget
function ui_menu:require_group_widget(parent, name)
    local w = ui_menu:require_widget(parent, name)
    w.onUpdate = function ()
        return true
    end
    return w
end