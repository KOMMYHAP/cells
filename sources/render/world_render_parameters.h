#pragma once

struct WorldRenderParameters {
    std::unique_ptr<sf::Font> font;
    std::unique_ptr<sf::Shader> shader;
};