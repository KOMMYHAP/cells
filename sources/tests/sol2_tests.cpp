#include <sol/sol.hpp>

#include "registrar/registrar.h"

#include "components/component_registry.h"
#include "systems/system_registry.h"

#include "native/lua_registrable_system.h"
#include "native/lua_system_impl.h"

class LuaSystemTestPreload final : public common::RegistrableSystem {
public:
    std::error_code InitializeSystem(common::StackStorage& storage) override
    {
        componentsRegistry = &storage.Store<ComponentRegistry>(cellsCount);
        systemsRegistry = &storage.Store<SystemRegistry>(*componentsRegistry);
        return {};
    }

    void TerminateSystem() override { }

    uint16_t cellsCount = 100;
    ComponentRegistry* componentsRegistry { nullptr };
    SystemRegistry* systemsRegistry { nullptr };
};

[[noreturn]] static sol::protected_function_result LuaPanicOnError(lua_State* L, sol::protected_function_result /*result*/)
{
    sol::error error = sol::stack::get_traceback_or_errors(L);
    ASSERT_FAIL(error.what());
}

TEST(SolTests, BasicUseCase)
{
    common::Registrar registrar;
    auto& preload = registrar.Register(std::make_unique<LuaSystemTestPreload>());
    auto& luaSystem = registrar.Register(std::make_unique<scripts::LuaSystem>());

    registrar.RunInit();

    auto& lua = luaSystem.ModifyImpl().ModifyLuaState();
    auto result = lua.safe_script(R"(
            function health_system(health)
                health = 100
            end

            health_component_id = register_component('health', 1)
            assert(health_component_id ~= invalid_component_id)
            health_system_id = register_sequence_system('set_health', health_system, health_component_id)
        )",
        LuaPanicOnError);

    const uint16_t rawHealthSystemId = lua["health_system_id"];
    const auto healthSystemId = static_cast<SystemHandle>(rawHealthSystemId);
    System& system = preload.systemsRegistry->Modify(healthSystemId);

    system.Foreach([]);

    registrar.RunTerm();

    //    sol::state lua {};
    //    lua.open_libraries(sol::lib::base);
    //    lua.new_usertype<scripts::LuaComponent>("component",
    //        "name", &scripts::LuaComponent::name,
    //        "size_in_bytes", &scripts::LuaComponent::sizeBytes);
    //    lua.new_usertype<scripts::ComponentId>("component_id", "id", sol::readonly(&scripts::ComponentId::id));
    //    lua.new_usertype<scripts::ComponentRegistry>("component_registry", "register", &scripts::ComponentRegistry::Register);
    //
    //    constexpr uint32_t cellsCount = 100;
    //    scripts::ComponentRegistry componentRegistry { cellsCount };
    //    lua["component_registry"] = &componentRegistry;
    //
    //    auto result = lua.safe_script(R"(
    //        function health_system(health)
    //            health = 100
    //        end
    //
    //        health_component_id = component_factory:make('health', 1)
    //        system_factory:register('set_health', health_system)
    //    )",
    //        scripts::PanicOnError);
    //
    //    componentRegistry._registry.Freeze();
    //
    //    ASSERT(result.valid(), result.status());
    //
    //    const scripts::ComponentId testComponentId = lua["test_component_id"];
    //    const ::ComponentHandle testComponentHandle = static_cast<::ComponentHandle>(testComponentId.id);
    //    const ComponentStorage& storage = componentRegistry._registry.Get(testComponentHandle);
    //
    //    const scripts::LuaComponent testComponent = lua["test_component"];
    //    ASSERT(storage.GetMetaInfo().sizeInBytes == testComponent.sizeBytes);
    //
    //    // how to create component in lua and pass it from c++?
}