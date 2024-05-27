#include "lua_component_factory.h"

namespace scripts {

constexpr uint8_t NumberSizeInBytes { 4 }; // 32 bits, 4 bytes

LuaComponentFactory::LuaComponentFactory(ComponentRegistry& componentRegistry)
    : _registry(componentRegistry)
{
}

ComponentHandle LuaComponentFactory::MakeComponent(std::string_view name, uint8_t numbersCount)
{
    Component component;
    component.name = name;
    component.sizeInBytes = numbersCount * NumberSizeInBytes;
    return _registry.Register(component);
}
}