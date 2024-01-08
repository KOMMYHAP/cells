#include <cstdint>
#include <limits>
#include <span>
#include <vector>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "brain.h"
#include "field.h"
#include "simulation.h"
#include "world_render.h"

int main()
{
    Cell emptyCell;
    emptyCell.GetBrain().AccessInfo().type = CellType::Dummy;

    Field field { 10, 10, emptyCell };
    Simulation simulation { field };
    WorldRender render { field };

    for (int i = 0; i < 10; ++i) {
        Cell cell;
        Brain brain = cell.GetBrain();
        brain.AccessPosition() = sf::Vector2<uint16_t>(i, 0);
        brain.AccessType() = CellType::Wall;
        field.Create(cell);

        brain.AccessPosition() = sf::Vector2<uint16_t>(9, i);
        field.Create(cell);

        brain.AccessPosition() = sf::Vector2<uint16_t>(i, 9);
        field.Create(cell);

        brain.AccessPosition() = sf::Vector2<uint16_t>(0, i);
        field.Create(cell);
    }

    for (int i = 1; i < 9; ++i) {
        Cell cell;
        Brain brain = cell.GetBrain();
        brain.AccessPosition() = sf::Vector2<uint16_t>(i, i);
        brain.AccessType() = CellType::Unit;
        field.Create(cell);
    }

    simulation.SetAutoUpdateMode(1);

    sf::RenderWindow window(sf::VideoMode(800, 600), "Cells");
    window.setFramerateLimit(60);

    sf::Clock clock;
    const sf::Color gray { 0xCCCCCCFF };

    while (window.isOpen()) {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event {};
        while (window.pollEvent(event)) {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }

        simulation.Update(clock.getElapsedTime());

        window.clear(gray);
        render.Render(window);
        window.display();
        clock.restart();
    }
    return 0;
}
