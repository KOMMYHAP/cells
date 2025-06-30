#pragma once

#include "SDL3/SDL_rect.h"

#include "widgets/custom_render_widget.h"

class World;
class WorldRasterizationTarget;
struct SDL_Texture;
struct SDL_Renderer;

class WorldWidget final : public CustomRenderWidget {
public:
    WorldWidget(World& world, SDL_Renderer& renderer, SDL_FRect textureRect);
    ~WorldWidget() override;

    void UpdateWidget(Common::Time elapsedTime) override;
    void RenderWidget() override;

    SDL_Texture& AccessRasterizationTexture() { return *_renderTargetTexture; }

private:
    SDL_Renderer* _renderer { nullptr };
    SDL_Texture* _renderTargetTexture { nullptr };
    SDL_FRect _textureRect;
    gsl::not_null<World*> _world;
};