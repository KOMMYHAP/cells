#include "world_widget.h"

#include "system/sdl_panic.h"
#include "world.h"
#include "world_rasterization_target.h"

WorldWidget::WorldWidget(World& world, SDL_Renderer& renderer, SDL_FRect textureRect)
    : _renderer(&renderer)
    , _textureRect(textureRect)
    , _world(&world)
{
    _renderTargetTexture = SDL_CreateTexture(
        _renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_STREAMING,
        static_cast<int32_t>(textureRect.w),
        static_cast<int32_t>(textureRect.h));
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
    if (SDL_RenderTexture(_renderer, _renderTargetTexture, nullptr, &_textureRect)) {
        PanicOnSdlError("SDL_RenderCopy"sv);
    }
}

void WorldWidget::UpdateWidget(Common::Time elapsedTime)
{
    _world->Update(elapsedTime);
}
