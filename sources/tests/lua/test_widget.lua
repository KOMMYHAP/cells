assert(ui_menu ~= nil)
assert(ui_menu.register ~= nil)
local w = ui_menu:register(ui_menu.root_widget, "my_test_widget")
if not w then
    error("failed to create widget")
    return
end

assert(w.name == "my_test_widget")
print(w)

w.onFirstTimeOpen = function() print("onFirstTimeOpen") end
w.onJustOpen = function() print("onJustOpen") end
w.onUpdate = function() print("onUpdate") return true end
w.onClosed = function() print("onClosed") end

ui_menu:open(w)
ui_menu:close(w)