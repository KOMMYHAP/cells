#pragma once

#include "drawable.h"
#include "updatable.h"

class MainWindow {
public:
    sf::RenderTarget* TryCreate(std::string_view title, sf::Vector2u size);

    struct RuntimeSetup {
        std::vector<Updatable*> updatableList;
        std::vector<Drawable*> drawableList;
    };
    void Run(RuntimeSetup runtimeSetup);

private:
    sf::RenderWindow _window;
};
