#pragma once

#include "SFML/Graphics/Texture.hpp"
#include "SFML/Graphics/VertexBuffer.hpp"

#include "ecs_simulation_system.h"

#include "components/cell_position.h"
#include "components/cell_type.h"

class RenderSystem final : public EcsSimulationSystem<RenderSystem, CellType, CellPosition> {
public:
    struct Config {
        gsl::not_null<EcsWorld*> ecsWorld;
        gsl::not_null<sf::Shader*> fragmentShader;
        sf::Vector2u fieldSize;
        sf::Vector2u renderTargetSize;
        sf::Vector2u renderTargetOffset;
    };
    explicit RenderSystem(Config&& config);

    void RenderFrame(sf::RenderTarget& target);

private:
    sf::Color GetColor(CellType type) const;
    void DoProcessComponents(CellId id, CellType type, CellPosition position);

    sf::Texture _texture;
    std::vector<uint32_t> _textureData;
    sf::VertexBuffer _vertexBuffer;
    gsl::not_null<sf::Shader*> _shader;
    sf::Vector2u _fieldSize;
    sf::Vector2u _renderTargetSize;
    sf::Vector2u _renderTargetOffset;
};
