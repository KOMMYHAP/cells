#pragma once

#include <SFML/Graphics/Color.hpp>

#include "simulation/simulation_ecs_system.h"

#include "components/cell_position.h"
#include "components/cell_type.h"

class RenderSystem final : public SimulationEcsSystem<RenderSystem, const CellType, const CellPosition> {
public:
    explicit RenderSystem(EcsWorld& ecsWorld, sf::Vector2u fieldSize);

    void ClearTextureData();
    std::span<const uint32_t> GetTextureData() const { return _textureData; }
    sf::Vector2u GetTextureSize() const { return _fieldSize; }

    void DoProcessComponents(EcsEntity id, CellType type, CellPosition position);
private:
    sf::Color GetColor(CellType type) const;

    std::vector<uint32_t> _textureData;
    sf::Vector2u _fieldSize;
};
