#include "render_system.h"

#include "SFML/Graphics/RenderTarget.hpp"

RenderSystem::RenderSystem(EcsWorld& ecsWorld, sf::Vector2u fieldSize)
    : SimulationEcsSystem(ecsWorld)
    , _fieldSize(fieldSize)
{
    _textureData.resize(_fieldSize.x * _fieldSize.y);
}

void RenderSystem::ClearTextureData()
{
    std::ranges::fill(_textureData, 0);
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

    auto* colorBytes = reinterpret_cast<std::byte*>(&_textureData[position.y * _fieldSize.x + position.x]);
    const sf::Color color = GetColor(type);
    colorBytes[0] = static_cast<std::byte>(color.r);
    colorBytes[1] = static_cast<std::byte>(color.g);
    colorBytes[2] = static_cast<std::byte>(color.b);
    colorBytes[3] = static_cast<std::byte>(color.a);
}