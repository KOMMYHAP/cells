#pragma once

#include "widgets/custom_render_widget.h"

class World;
class WorldRasterizationTarget;
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

    WorldWidget(World& world, SDL_Renderer& renderer, int32_t bytesPerCell, Rect textureRect);
    ~WorldWidget() override;

    void UpdateWidget(Common::Time elapsedTime) override;
    void RenderWidget() override;

    WorldRasterizationTarget& AccessRasterizationTarget() { return *_rasterizationTarget; }

private:
    SDL_Renderer* _renderer { nullptr };
    SDL_Texture* _renderTargetTexture { nullptr };
    SDL_PixelFormat* _texturePixelFormat { nullptr };
    Rect _textureRect;
    gsl::not_null<World*> _world;
    std::unique_ptr<WorldRasterizationTarget> _rasterizationTarget;
};