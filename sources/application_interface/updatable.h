#pragma once

#include <SFML/System/Time.hpp>

class Updatable {
public:
    virtual ~Updatable() = default;
    virtual void Update(sf::Time elapsedTime) = 0;
};