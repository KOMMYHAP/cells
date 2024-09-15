#include "world_widget.h"

#include "system/sdl_panic.h"

WorldWidget::WorldWidget(SDL_Renderer& renderer, World& world, WorldRasterizationSystem& worldRasterizationSystem, SDL_Rect textureRect)
    : _world(&world)
    , _renderer(&renderer)
    , _textureRect(textureRect)
    , _rasterizationSystem(&worldRasterizationSystem)
{
    _texture = SDL_CreateTexture(
        _renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_STREAMING,
        NarrowCast<int>(textureRect.w),
        NarrowCast<int>(textureRect.h));
    if (!_texture) {
        PanicOnSdlError("SDL_CreateTexture"sv);
    }

    uint32_t pixelFormat { 0 };
    if (SDL_QueryTexture(_texture, &pixelFormat, nullptr, nullptr, nullptr)) {
        PanicOnSdlError("SDL_QueryTexture"sv);
    }
    _texturePixelFormat = SDL_AllocFormat(pixelFormat);
    if (!_texturePixelFormat) {
        PanicOnSdlError("SDL_AllocFormat"sv);
    }
}

WorldWidget::~WorldWidget()
{
    SDL_FreeFormat(_texturePixelFormat);
    SDL_DestroyTexture(_texture);
}

void WorldWidget::RenderWidget()
{
    if (SDL_RenderCopy(_renderer, _texture, nullptr, &_textureRect)) {
        PanicOnSdlError("SDL_RenderCopy"sv);
    }
}

void WorldWidget::UpdateWidget(Common::Time elapsedTime)
{
    void* pixels { nullptr };
    int pitch { 0 };
    if (SDL_LockTexture(_texture, nullptr, &pixels, &pitch)) {
        PanicOnSdlError("SDL_LockTexture"sv);
    }
    const WorldRasterizationData data {
        static_cast<std::byte*>(pixels),
        NarrowCast<size_t>(pitch) * _textureRect.h,
        NarrowCast<uint32_t>(pitch),
        _texturePixelFormat
    };
    _rasterizationSystem->SetDestination(data);

    _world->Update(elapsedTime);
    _rasterizationSystem->DoSystemUpdate();

    _rasterizationSystem->ResetDestination();
    SDL_UnlockTexture(_texture);
}
