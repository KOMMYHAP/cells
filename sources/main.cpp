#include "field.h"

void PrintWorld(const Field& w)
{
    for (auto& c : w) {
        std::cout << std::format("({0}; {1}\n", c.position.x, c.position.y);
    }
    std::cout << std::endl;
}



int main()
{
    Field field { 5, 20 };

    auto id1 = field.Create(Cell { {}, sf::Vector2u { 1, 1 } });
    auto id2 = field.Create(Cell { {}, sf::Vector2u { 1, 2 } });
    auto id3 = field.Create(Cell { {}, sf::Vector2u { 1, 3 } });
    auto id4 = field.Create(Cell { {}, sf::Vector2u { 2, 2 } });
    PrintWorld(field);

    field.Remove(id1);
    PrintWorld(field);

    auto& cell = field.Get(id2);
    PrintWorld(field);

    field.Move(id3, Cell { {}, sf::Vector2u { 3, 3 } });
    PrintWorld(field);

    auto cells = field.Find({ 3, 3 });
    auto id5 = field.Create(Cell { {}, sf::Vector2u { 3, 3 } });
    cells = field.Find({ 3, 3 });

    sf::Window window(sf::VideoMode(800, 600), "My window");
    window.setFramerateLimit(60);

    while (window.isOpen()) {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event)) {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.display();
    }

    return 0;
}
