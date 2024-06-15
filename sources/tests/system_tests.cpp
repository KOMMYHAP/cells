#include "components/component_registry.h"
#include "systems/sequence_system.h"
#include "systems/system_registry.h"

struct TestComponent {
    int value;
};

TEST(SystemTests, Foreach)
{
    // constexpr size_t itemsCount = 10;
    // ComponentRegistry registry { itemsCount };
    // const ComponentHandle handle = registry.Register(Component { "test", sizeof(TestComponent) });
    // registry.Freeze();
    //
    // registry.Modify(handle).Foreach<TestComponent>([](TestComponent& component) {
    //     component.value = 0;
    // });
    //
    // SystemRegistry systemRegistry { registry };
    //
    // std::array components { handle };
    //
    // auto initComponent = [](const SystemContext& context) {
    //     ASSERT(context.components.size() == 1);
    //     auto* component = reinterpret_cast<TestComponent*>(context.components[0]);
    //     component->value = 1;
    // };
    // const auto testSystem = systemRegistry.MakeSequenceSystem("test", std::span { components }, initComponent);
    //
    // // system.Foreach();
    //
    // registry.Get(handle).Foreach<TestComponent>([](const TestComponent& component) {
    //     ASSERT_EQ(component.value, 1);
    // });
    //
    // int idx = 0;
    // system.Foreach([&idx](const SystemContext& context) {
    //     ASSERT(context.components.size(), 1);
    //     auto* component = reinterpret_cast<TestComponent*>(context.components[0]);
    //     idx += 1;
    //     component->value = idx;
    // });
    //
    // idx = 0;
    // registry.Get(handle).Foreach<TestComponent>([&idx](const TestComponent& component) {
    //     idx += 1;
    //     ASSERT_EQ(component.value, idx);
    // });
}

TEST(SystemTests, Message)
{
    // constexpr size_t itemsCount = 10;
    // ComponentRegistry registry { itemsCount };
    // const ComponentHandle handle = registry.Register(Component { "test", sizeof(TestComponent) });
    // registry.Freeze();
    //
    // registry.Modify(handle).Foreach<TestComponent>([](TestComponent& component) {
    //     component.value = 0;
    // });
    //
    // std::array components { handle };
    // System system { "test", registry, components };
    // std::array<CellId, 3> ids { CellId { 0 }, CellId { 3 }, CellId { 9 } };
    // system.Message(ids, [](const System::Context& context) {
    //     ASSERT(context.components.size(), 1);
    //     auto* component = reinterpret_cast<TestComponent*>(context.components[0]);
    //     component->value = static_cast<int>(context.id);
    // });
    //
    // system.Foreach([&ids](const System::Context& context) {
    //     ASSERT(context.components.size(), 1);
    //     auto* component = reinterpret_cast<TestComponent*>(context.components[0]);
    //     const bool shouldBeChanged = std::find(ids.begin(), ids.end(), context.id) != ids.end();
    //     if (!shouldBeChanged) {
    //         return;
    //     }
    //     ASSERT_EQ(component->value, static_cast<int>(context.id));
    // });
}