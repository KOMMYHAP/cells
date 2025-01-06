assert(ui_menu ~= nil)
assert(ui_menu.register ~= nil)
local id, w = ui_menu:register(nil, "my_test_widget")
assert(id ~= ui_menu.invalid_widget)
assert(w ~= nil)
assert(w.name == "my_test_widget")
print(id)
print(w)

w.onFirstTimeOpen = function() print("onFirstTimeOpen") end
w.onJustOpen = function() print("onJustOpen") end
w.onUpdate = function() print("onUpdate") return true end
w.onClosed = function() print("onClosed") end