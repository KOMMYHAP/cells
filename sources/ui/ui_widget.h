#pragma once

class UiWidget {
public:
    virtual ~UiWidget() = default;
    virtual void Update(sf::Time elapsedTime) = 0;
    virtual void Draw(sf::RenderTarget& target) = 0;
};