assert(ui_menu ~= nil)
assert(ui_menu.register ~= nil)
local w = ui_menu:register(ui_menu.root_widget, "my_test_widget")
if not w then
    error("failed to create widget")
    return nil
end

assert(w.name == "my_test_widget")
print(w)

---@param self menu_widget
---@param dt number
local function on_update(self, dt)
    print(string.format("self = %p, dt = %d, name = %s", self, dt, self.name))
end

w.onFirstTimeOpen = function() print("onFirstTimeOpen") end
w.onJustOpen = function() print("onJustOpen") end
w.onUpdate = function(dt) on_update(w, dt) return true end
w.onClosed = function() print("onClosed") end
return w