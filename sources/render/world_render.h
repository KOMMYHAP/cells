#pragma once

#include "cell_render.h"
#include "field.h"

class WorldRender {
public:
    WorldRender(Field& field, CellRender::Config cellRenderConfig);

    void Render(sf::RenderTarget& target);

private:
    const Field& _field;
    CellRender _cellRender;
};