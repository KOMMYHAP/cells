#pragma once

#include "components/cell_id.h"
#include "components/cell_type.h"
#include "ui_widget.h"

class PositionSystem;
class IdSystem;
class TypeSystem;

class WorldWidget : public UiWidget {
public:
    struct Config {
        std::unique_ptr<sf::Shader> fragmentShader;
        std::vector<sf::Color> colors;
        sf::Vector2u renderTargetSize;
        sf::Vector2u renderTargetOffset;

        PositionSystem& positionSystem;
        IdSystem& idSystem;
        TypeSystem& typeSystem;
    };

    WorldWidget(Config&& config);

    void Update(sf::Time elapsedTime) override;
    void Draw(sf::RenderTarget& target) override;

private:
    void ProcessCell(CellId id);
    sf::Color GetColor(CellType type) const;

    Config _config;
    sf::Texture _texture;
    std::vector<uint32_t> _textureData;
    sf::VertexBuffer _vertexBuffer;
};