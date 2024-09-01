#pragma once

#include "widgets/custom_render_widget.h"
#include "world_rasterization_system.h"

class WorldWidget final : public CustomRenderWidget {
public:
    WorldWidget(SDL_Renderer& renderer, WorldRasterizationSystem& worldRasterizationSystem, SDL_Rect textureRect);
    ~WorldWidget() override;

    void UpdateWidget(sf::Time elapsedTime) override;
    void RenderWidget() override;

private:
    SDL_Renderer* _renderer { nullptr };
    SDL_Texture* _texture { nullptr };
    SDL_PixelFormat* _texturePixelFormat { nullptr };
    SDL_Rect _textureRect;
    gsl::not_null<WorldRasterizationSystem*> _rasterizationSystem;
};