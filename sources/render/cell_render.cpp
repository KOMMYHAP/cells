#include "cell_render.h"
#include "brain.h"
#include "cell.h"

CellRender::CellRender(Config config)
    : _config(std::move(config))
{
    // unit + wall + food
    assert(_config.colors.size() == 3);
}

void CellRender::Render(sf::RenderTarget& target, const Cell& cell)
{
    const BrainInfo& info = ConstBrain(cell).GetInfo();
    const auto type = info.type;
    if (type == CellType::Dummy) {
        return;
    }

    sf::RectangleShape shape;
    shape.setSize({ _config.cellSize, _config.cellSize });

    const sf::Color color = _config.colors[static_cast<int>(type)];
    shape.setFillColor(color);
    shape.setOutlineColor(color);

    const float x = _config.fieldOffset.x + (_config.paddingLeft + _config.cellSize) * static_cast<float>(info.position.x);
    const float y = _config.fieldOffset.y + (_config.paddingTop + _config.cellSize) * static_cast<float>(info.position.y);
    shape.setPosition(x, y);

    target.draw(shape);
}