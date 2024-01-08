#include "world.h"

void PrintWorld(const World& w)
{
    for (auto& c : w) {
        std::cout << std::format("({0}; {1}): {2}\n", c.position.x, c.position.y, TypeToStr(c.type));
    }
    std::cout << std::endl;
}

int main()
{
    World w { 5, 20 };

    auto id1 = w.Create(Cell { Type::Unit, sf::Vector2u { 1, 1 } });
    auto id2 = w.Create(Cell { Type::Unit, sf::Vector2u { 1, 2 } });
    auto id3 = w.Create(Cell { Type::Unit, sf::Vector2u { 1, 3 } });
    auto id4 = w.Create(Cell { Type::Food, sf::Vector2u { 2, 2 } });
    PrintWorld(w);

    w.Remove(id1);
    PrintWorld(w);

    auto& cell = w.Get(id2);
    PrintWorld(w);

    w.Update(id3, Cell { Type::Wall, sf::Vector2u { 3, 3 } });
    PrintWorld(w);

    auto cells = w.Find({ 3, 3 });
    auto id5 = w.Create(Cell { Type::Unit, sf::Vector2u { 3, 3 } });
    cells = w.Find({ 3, 3 });

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
