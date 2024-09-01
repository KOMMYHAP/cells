#pragma once

class BaseWidget {
public:
    virtual ~BaseWidget() = default;
    virtual void UpdateWidget(sf::Time elapsedTime) = 0;
};