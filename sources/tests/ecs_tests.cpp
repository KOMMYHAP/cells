#include "systems_ecs/simulation_ecs_system.h"

struct position {
    uint32_t x;
    uint32_t y;
};

struct velocity {
    uint32_t dx;
    uint32_t dy;
};

TEST(ECS_TESTS, Sample)
{
    entt::registry registry;

    for (auto i = 0u; i < 10u; ++i) {
        const auto entity = registry.create();
        registry.emplace<position>(entity, i, i);
        if (i % 2 == 0) {
            registry.emplace<velocity>(entity, i * 10, i * 10);
        }
    }

    auto view = registry.view<const position, velocity>();

    // use an extended callback
    view.each([](const auto entity, const auto& pos, auto& vel) {
        ASSERT_EQ(static_cast<uint32_t>(entity), pos.x);
        ASSERT_EQ(static_cast<uint32_t>(entity), pos.y);
        ASSERT_EQ(static_cast<uint32_t>(entity) * 10, vel.dx);
        ASSERT_EQ(static_cast<uint32_t>(entity) * 10, vel.dy);
    });
}