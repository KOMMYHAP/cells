#pragma once

#include <SFML/Graphics/Color.hpp>

#include "ecs_simulation_system.h"

#include "components/cell_position.h"
#include "components/cell_type.h"

class RenderSystem final : public EcsSimulationSystem<RenderSystem, CellType, CellPosition> {
public:
    explicit RenderSystem(EcsWorld& ecsWorld, sf::Vector2u fieldSize);

    std::span<const uint32_t> GetTextureData() const { return _textureData; }
    sf::Vector2u GetTextureSize() const { return _fieldSize; }

private:
    sf::Color GetColor(CellType type) const;
    void DoProcessComponents(CellId id, CellType type, CellPosition position);

    std::vector<uint32_t> _textureData;
    sf::Vector2u _fieldSize;
};
