#include "systems/system.h"
#include "components/component_registry.h"

struct TestComponent {
    int value;
};

TEST(SystemTests, Case)
{
    constexpr size_t itemsCount = 10;
    ComponentRegistry registry { itemsCount };
    const ComponentHandle handle = registry.Register(Component { sizeof(TestComponent) });
    registry.Freeze();

    registry.Modify(handle).Foreach<TestComponent>([](TestComponent& component) {
        component.value = 0;
    });

    System system { registry, { handle } };
    system.Foreach([](const System::Context& context) {
        ASSUME(context.components.size(), 1);
        auto* component = reinterpret_cast<TestComponent*>(context.components[0]);
        component->value = 1;
    });

    registry.Get(handle).Foreach<TestComponent>([](const TestComponent& component) {
        ASSERT_EQ(component.value, 1);
    });

    int idx = 0;
    system.Foreach([&idx](const System::Context& context) {
        ASSUME(context.components.size(), 1);
        auto* component = reinterpret_cast<TestComponent*>(context.components[0]);
        idx += 1;
        component->value = idx;
    });

    idx = 0;
    registry.Get(handle).Foreach<TestComponent>([&idx](const TestComponent& component) {
        idx += 1;
        ASSERT_EQ(component.value, idx);
    });
}