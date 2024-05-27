#include "lua_system_impl.h"

#include "storage/stack_storage.h"

namespace scripts {

LuaSystemImpl::LuaSystemImpl(common::StackStorage& storage)
    : systemFactory(storage.Modify<SystemRegistry>())
    , componentFactory(storage.Modify<ComponentRegistry>())
{
    sol::state& lua = luaState;
    lua.open_libraries(sol::lib::base);

    lua.set_function("register_component", [factory = &componentFactory](std::string_view name, uint8_t numbersCount) {
        const ComponentHandle handle = factory->MakeComponent(name, numbersCount);
        return static_cast<uint16_t>(handle);
    });

    lua.set_function("register_sequence_system", [factory = &systemFactory](std::string_view name, sol::variadic_args args) {
        const SystemHandle handle = factory->MakeSystem(name, std::move(args));
        return static_cast<uint16_t>(handle);
    });

    lua["invalid_component_id"] = static_cast<uint16_t>(ComponentHandle::Invalid);
    lua["invalid_system_id"] = static_cast<uint16_t>(SystemHandle::Invalid);
}

}
