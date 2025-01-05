local id, w = ui_menu.register(nil, "test")
assert(id ~= ui_menu.invalid_widget)
assert(w ~= nil)
print(id)
print(w)

local id2, w2 = ui_menu.register(42, "test")
assert(id2 == ui_menu.invalid_widget)
assert(w2 == nil)

w.onFirstTimeOpen = function() print("onFirstTimeOpen") end
w.onJustOpen = function() print("onJustOpen") end
w.onUpdate = function() print("onUpdate") return true end
w.onClosed = function() print("onClosed") end