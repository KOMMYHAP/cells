#include <span>
#include <vector>
#include <limits>
#include <cstdint>

#include <SFML/Window.hpp>

#include "field.h"
#include "simulation.h"

int main()
{
    Field field { 10, 10 };
    Simulation simulation { field };

    simulation.SetAutoUpdateMode(1);

    sf::Window window(sf::VideoMode(800, 600), "Cells");
    window.setFramerateLimit(60);

    sf::Clock clock;

    while (window.isOpen()) {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event{};
        while (window.pollEvent(event)) {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }

        simulation.Update(clock.getElapsedTime());

        window.display();
        clock.restart();
    }
    return 0;
}
