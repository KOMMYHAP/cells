#pragma once

// ECS Framework 'entt' doest not support empty component (e.g. tags), so when you need such tag component you can inherit it from DummyComponent.
struct DummyComponent {
    bool stub;
};