#include "main_window.h"

sf::RenderTarget* MainWindow::TryCreate(std::string_view title, sf::Vector2u size)
{
    if (_window.isOpen()) {
        return nullptr;
    }

    const sf::String sfmlTitle = sf::String(std::string(title.data(), title.length()));
    _window.create(sf::VideoMode(size.x, size.y), sfmlTitle, sf::Style::Titlebar | sf::Style::Close);
    if (!_window.isOpen()) {
        return nullptr;
    }
    _window.setVerticalSyncEnabled(false);
    _window.setFramerateLimit(60);
    return &_window;
}

void MainWindow::Run(RuntimeSetup runtimeSetup)
{
    sf::Clock frameClock;
    while (_window.isOpen()) {
        sf::Event event {};
        while (_window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                _window.close();
            }
        }

        for (Updatable* updatable : runtimeSetup.updatableList) {
            updatable->Update(frameClock.getElapsedTime());
        }

        if (_window.isOpen()) {
            const sf::Color gray { 0xCCCCCCFF };
            _window.clear(gray);

            for (Drawable* drawable : runtimeSetup.drawableList) {
                drawable->Draw();
            }

            _window.display();
        }

        frameClock.restart();
    }
}
