#include "lua_system.h"

#include "bind/lua_menu.h"
#include "bind/lua_menu_widget.h"

LuaSystem::LuaSystem(MenuRootWidget& menuRootWidget)
    : _menuRootWidget(&menuRootWidget)
{
    _luaState.open_libraries(sol::lib::base);

    _luaState.new_usertype<LuaMenuWidget>("menu_widget",
        sol::meta_function::new_index,
        &LuaMenuWidget::OverrideFunction);

    _luaState.new_usertype<LuaMenu>("menu", "register", &LuaMenu::Register);
    _luaState["ui_menu"] = LuaMenu(_logger, *_menuRootWidget);

        static constexpr auto script = R"(
id, w = ui_menu:register(nil, "test")
assert(w ~= nil)
w.onFirstTimeOpen = function() print("onFirstTimeOpen") end
w.onJustOpen = function() print("onJustOpen") end
w.onUpdate = function() print("onUpdate") return true end
w.onClosed = function() print("onClosed") end
print(id)
print(w)

id2, w2 = ui_menu:register(42, "test")
assert(w2 == nil)
)"sv;
    const auto r = _luaState.script(script, sol::script_pass_on_error);
    if (!r.valid()) {
        const sol::error error = r;
        _logger.Error("Failed to run script: {}", error.what());
    }
}