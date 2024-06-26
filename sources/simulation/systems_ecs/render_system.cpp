#include "render_system.h"

#include "SFML/Graphics/RenderTarget.hpp"

RenderSystem::RenderSystem(Config&& config)
    : EcsSimulationSystem(*config.ecsWorld)
    , _shader(config.fragmentShader)
    , _fieldSize(config.fieldSize)
    , _renderTargetSize(config.renderTargetSize)
    , _renderTargetOffset(config.renderTargetOffset)
{
}

void RenderSystem::RenderFrame(sf::RenderTarget& target)
{
    const uint32_t clearColor = GetColor(CellType::Dummy).toInteger();
    std::ranges::fill(_textureData, clearColor);

    _texture.update(reinterpret_cast<const uint8_t*>(_textureData.data()));

    sf::RenderStates states;
    states.shader = _shader.get();
    states.transform.translate(NarrowCast<float>(_renderTargetOffset.x), NarrowCast<float>(_renderTargetOffset.y));
    target.draw(_vertexBuffer, states);
}

sf::Color RenderSystem::GetColor(const CellType type) const
{
    switch (type) {
    case CellType::Unit:
        return sf::Color::Red;
    case CellType::Food:
        return sf::Color::Yellow;
    case CellType::Wall:
        return sf::Color::Black;
    case CellType::Dummy:
        return sf::Color::White;
    }
    return sf::Color::Magenta;
}

void RenderSystem::DoProcessComponents(CellId /*id*/, const CellType type, const CellPosition position)
{
    const auto pixelIndex = position.y * _fieldSize.x + position.x;
    ASSERT(pixelIndex < _textureData.size());

    uint32_t& pixel = _textureData[position.y * _fieldSize.x + position.x];
    pixel = GetColor(type).toInteger();
}