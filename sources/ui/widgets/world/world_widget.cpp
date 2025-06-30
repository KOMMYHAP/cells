#include "world_widget.h"

#include "system/sdl_panic.h"
#include "world.h"
#include "world_rasterization_target.h"

WorldWidget::WorldWidget(World& world, SDL_Renderer& renderer, Rect textureRect)
    : _renderer(&renderer)
    , _textureRect(textureRect)
    , _world(&world)
{
    _renderTargetTexture = SDL_CreateTexture(
        _renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_STREAMING,
        textureRect.width,
        textureRect.height);
    if (!_renderTargetTexture) {
        PanicOnSdlError("SDL_CreateTexture"sv);
    }
}

WorldWidget::~WorldWidget()
{
    SDL_DestroyTexture(_renderTargetTexture);
}

void WorldWidget::RenderWidget()
{
    const SDL_Rect rect { _textureRect.x, _textureRect.y, _textureRect.width, _textureRect.height };
    if (SDL_RenderCopy(_renderer, _renderTargetTexture, nullptr, &rect)) {
        PanicOnSdlError("SDL_RenderCopy"sv);
    }
}

void WorldWidget::UpdateWidget(Common::Time elapsedTime)
{
    _world->Update(elapsedTime);
}
