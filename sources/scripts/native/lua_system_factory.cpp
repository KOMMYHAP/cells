#include "lua_system_factory.h"
#include "sol/types.hpp"

namespace scripts {

LuaSystemFactory::LuaSystemFactory(SystemRegistry& systemRegistry, ComponentRegistry& componentRegistry)
    : _registry(systemRegistry)
    , _componentRegistry(componentRegistry)
{
}

SystemHandle LuaSystemFactory::MakeSystem(sol::this_state state, std::string_view name, sol::protected_function function, const sol::variadic_args& args)
{
    auto storage = std::make_unique<LuaSystemStorage>(*this, std::vector<Component> {}, state, std::move(function));
    std::vector<ComponentHandle> handles;

    storage->components.reserve(args.size());
    handles.reserve(args.size());

    for (sol::variadic_args::const_iterator arg : args) {
        if (!arg->is<uint16_t>()) {
            PANIC(std::format("Failed to register system: argument at {} index must be component handle!", arg.index));
            return SystemHandle::Invalid;
        }

        const uint16_t rawHandle = arg->as<uint16_t>();
        const ComponentHandle handle { rawHandle };
        if (handle == ComponentHandle::Invalid) {
            PANIC(std::format("Failed to register system: argument at {} index is invalid handle, seems like its component was not created!", arg.index));
            return SystemHandle::Invalid;
        }

        const Component& component = _componentRegistry.Get(handle).GetMetaInfo();
        storage->components.push_back(component);

        handles.push_back(handle);
    }

    auto luaFunctionWrapper = [s = std::move(storage)](const SystemContext& context) {
        storage->
        ProcessSystem()
    };

    return _registry.MakeSequenceSystem(name, handles, std::move(luaFunctionWrapper));
}

void LuaSystemFactory::ProcessSystem(LuaSystemFactory::LuaSystemStorage& storage, const SystemContext& context)
{
    int pushcount = 0;
    for (int i = index; i <= stacktop; ++i) {
        lua_pushvalue(L, i);
        pushcount += 1;
    }
    return pushcount;
}

}