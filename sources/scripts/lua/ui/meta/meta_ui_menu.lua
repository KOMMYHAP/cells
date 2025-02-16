---@meta cells_ui

---@class ui_native
---@field root_widget number Special value for root widget
---@field invalid_widget number Special value for invalid widget
ui_native = {
}


---@class menu_widget
---@field onFirstTimeOpen fun()
---@field onJustOpen fun()
---@field onUpdate fun(dt: integer): boolean Returns true if stil opened, otherwise returns false
---@field onClosed fun()
---@field name string
---@field id number
local menu_widget = {}


---@param parent_id number
---@param name string
---@return menu_widget? widget
function ui_native:register(parent_id, name)  end

---@param widget menu_widget
function ui_native:open(widget)  end

---@param widget menu_widget
function ui_native:close(widget)  end