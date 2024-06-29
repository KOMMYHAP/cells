#pragma once

#include "systems_ecs/render_system.h"
#include "ui_widget.h"

class WorldWidget : public UiWidget {
public:
    struct Config {
        gsl::not_null<RenderSystem*> renderSystem;
        std::unique_ptr<sf::Shader> fragmentShader;
        sf::Vector2u renderTargetSize;
        sf::Vector2u renderTargetOffset;
    };

    WorldWidget(Config&& config);

    void Draw(sf::RenderTarget& target) override;
    void Update(sf::Time elapsedTime) override;

private:
    Config _config;
    sf::Texture _texture;
    sf::VertexBuffer _vertexBuffer;
};