#pragma once

#include "widgets/custom_render_widget.h"
#include "world.h"
#include "world_rasterization_system.h"

struct SDL_Texture;
struct SDL_Renderer;
struct SDL_PixelFormat;

class WorldWidget final : public CustomRenderWidget {
public:
    struct Rect {
        int x { 0 };
        int y { 0 };
        int width { 0 };
        int height { 0 };
    };

    WorldWidget(World& world, SDL_Renderer& renderer, WorldRasterizationSystem& worldRasterizationSystem, Rect textureRect);
    ~WorldWidget() override;

    void UpdateWidget(Common::Time elapsedTime) override;
    void RenderWidget() override;

private:
    SDL_Renderer* _renderer { nullptr };
    SDL_Texture* _texture { nullptr };
    SDL_PixelFormat* _texturePixelFormat { nullptr };
    Rect _textureRect;
    gsl::not_null<World*> _world;
    gsl::not_null<WorldRasterizationSystem*> _rasterizationSystem;
};