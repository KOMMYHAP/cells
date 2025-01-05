assert(ui_menu ~= nil)
assert(ui_menu.register ~= nil)
local id, w = ui_menu:register(nil, "test")
assert(id ~= ui_menu.invalid_widget)
assert(w ~= nil)
print(id)
print(w)

w.onFirstTimeOpen = function() print("onFirstTimeOpen") end
w.onJustOpen = function() print("onJustOpen") end
w.onUpdate = function() print("onUpdate") return true end
w.onClosed = function() print("onClosed") end