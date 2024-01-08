#include "cell_render.h"
#include "brain.h"
#include "cell.h"

CellRender::CellRender()
    : _paddingLeft(8.0f)
    , _paddingTop(8.0f)
    , _cellSize(16.0f)
    , _fieldOffset(25.0f, 25.0f)
{
    _colors.resize(3);
    _colors[static_cast<int>(CellType::Unit)] = sf::Color(sf::Color::Green);
    _colors[static_cast<int>(CellType::Food)] = sf::Color(sf::Color::Red);
    _colors[static_cast<int>(CellType::Wall)] = sf::Color(sf::Color::Black);
}

void CellRender::Render(sf::RenderTarget& target, const Cell& cell)
{
    const auto type = BrainView(cell).GetType();
    if (type == CellType::Dummy) {
        return;
    }

    sf::RectangleShape shape;
    shape.setSize({ _cellSize, _cellSize });

    const sf::Color color = _colors[static_cast<int>(type)];
    shape.setFillColor(color);

    const float x = _fieldOffset.x + (_paddingLeft + _cellSize) * static_cast<float>(cell.GetBrain().GetPosition().x);
    const float y = _fieldOffset.y + (_paddingTop + _cellSize) * static_cast<float>(cell.GetBrain().GetPosition().y);
    shape.setPosition(x, y);

    target.draw(shape);
}