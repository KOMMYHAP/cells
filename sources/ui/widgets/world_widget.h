#pragma once

#include "custom_render_widget.h"
#include "systems_ecs/render_system.h"

class WorldWidget final : public CustomRenderWidget {
public:
    struct Config {
        gsl::not_null<RenderSystem*> renderSystem;
        std::unique_ptr<sf::Shader> fragmentShader;
        sf::Vector2u renderTargetSize;
        sf::Vector2u renderTargetOffset;
    };

    WorldWidget(Config&& config);

    void RenderWidget(sf::RenderTarget& target) override;
    void UpdateWidget(sf::Time elapsedTime) override;

private:
    Config _config;
    sf::Texture _texture;
    sf::VertexBuffer _vertexBuffer;
};