#include <cstdint>
#include <limits>
#include <span>
#include <vector>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "brain.h"
#include "field.h"
#include "simulation.h"
#include "unit_processor.h"
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
        BrainInfo& info = brain.AccessInfo();
        info.position = sf::Vector2<uint16_t>(i, 0);
        info.type = CellType::Wall;
        field.Create(cell);

        info.position = sf::Vector2<uint16_t>(9, i);
        field.Create(cell);

        info.position = sf::Vector2<uint16_t>(i, 9);
        field.Create(cell);

        info.position = sf::Vector2<uint16_t>(0, i);
        field.Create(cell);
    }

    {
        Cell cell = UnitProcessor::MakeDefaultUnit();
        Brain brain = cell.GetBrain();
        brain.AccessInfo().position = sf::Vector2<uint16_t>(1, 1);

        BrainData dataScope = brain.AccessData();
        UnitControlBlock& controlBlock = dataScope.Pop<UnitControlBlock>();

        for (int i = 0; i < 7; ++i) {
            UnitCommandParam& move = dataScope.Pop<UnitCommandParam>();
            move.value = static_cast<std::underlying_type_t<UnitCommand>>(UnitCommand::Move);
            Direction& direction = dataScope.Pop<Direction>();
            direction = Direction::Right;
        }
        for (int i = 0; i < 7; ++i) {
            UnitCommandParam& move = dataScope.Pop<UnitCommandParam>();
            move.value = static_cast<std::underlying_type_t<UnitCommand>>(UnitCommand::Move);
            Direction& direction = dataScope.Pop<Direction>();
            direction = Direction::Left;
        }
        UnitCommandParam& jump = dataScope.Pop<UnitCommandParam>();
        jump.value = static_cast<std::underlying_type_t<UnitCommand>>(UnitCommand::Jump);
        UnitCommandParam& destination = dataScope.Pop<UnitCommandParam>();
        destination.value = 0;

        field.Create(cell);
    }

        for (int i = 2; i < 9; ++i) {
            Cell cell = UnitProcessor::MakeDefaultUnit();
            cell.GetBrain().AccessInfo().position = sf::Vector2<uint16_t>(i, i);
            field.Create(cell);
        }

    simulation.SetAutoUpdateMode(1);

    sf::RenderWindow window(sf::VideoMode(800, 600), "Cells");
    window.setFramerateLimit(60);

    sf::Clock clock;
    const sf::Color gray { 0xCCCCCCFF };

    while (window.isOpen()) {
        const sf::Time elapsedTime = clock.getElapsedTime();
        clock.restart();

        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event {};
        while (window.pollEvent(event)) {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }

        simulation.Update(elapsedTime);

        window.clear(gray);
        render.Render(window);
        window.display();
    }
    return 0;
}
