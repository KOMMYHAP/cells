// #include "components/component_registry.h"
// #include "systems/sequence_system.h"
// #include "systems/system_factory.h"
// #include "systems/system_registry.h"
//
// class SystemFixture : public ::testing::Test {
// public:
//     SystemFactory* systemFactory { nullptr };
//     SystemRegistry* systemRegistry { nullptr };
//     ComponentRegistry* componentRegistry { nullptr };
//     uint32_t cellsCount { 0 };
//
// protected:
//     void SetUp() override
//     {
//         cellsCount = 100;
//         _componentRegistry = std::make_unique<ComponentRegistry>(cellsCount);
//         _systemRegistry = std::make_unique<SystemRegistry>(*_componentRegistry);
//         _systemFactory = std::make_unique<SystemFactory>(*_systemRegistry, *_componentRegistry);
//
//         systemFactory = _systemFactory.get();
//         componentRegistry = _componentRegistry.get();
//         systemRegistry = _systemRegistry.get();
//     }
//     void TearDown() override
//     {
//         systemRegistry = nullptr;
//         componentRegistry = nullptr;
//         systemFactory = nullptr;
//         _systemFactory.reset();
//         _systemRegistry.reset();
//         _componentRegistry.reset();
//     }
//
// private:
//     std::unique_ptr<ComponentRegistry> _componentRegistry;
//     std::unique_ptr<SystemRegistry> _systemRegistry;
//     std::unique_ptr<SystemFactory> _systemFactory;
// };
//
// TEST_F(SystemFixture, SetFixedValue)
// {
//     struct Value {
//         int data;
//     };
//     constexpr int TestValue = 42;
//
//     const ComponentHandle component = componentRegistry->Register({ "Value", 1, sizeof(Value) });
//     std::span components { &component, 1 };
//     const SystemHandle system = systemFactory->Make("SetFixedValue", components, [](const SystemContext& context) {
//         std::byte* rawComponent = context.components[0];
//         auto* value = reinterpret_cast<Value*>(rawComponent);
//         value->data = TestValue;
//     });
//
//     SystemBase& s = systemRegistry->Modify(system);
//     s.Process();
// }

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