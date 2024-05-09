#pragma once

#include "drawable.h"

class World;
class WorldRender;

class WorldWidget : public Drawable {
public:
    WorldWidget(sf::RenderTarget& renderTarget, WorldRender& worldRender, sf::Vector2f renderAreaOffset);
    ~WorldWidget();

    void Draw() override;

private:
    sf::RenderTarget& _renderTarget;
    sf::RenderStates _rootStates;
    WorldRender& _worldRender;
};