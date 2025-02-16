#include "world_widget.h"

#include "system/sdl_panic.h"
#include "world_rasterization_data.h"

WorldWidget::WorldWidget(World& world, SDL_Renderer& renderer, WorldRasterizationSystem& worldRasterizationSystem, Rect textureRect)
    : _renderer(&renderer)
    , _textureRect(textureRect)
    , _world(&world)
    , _rasterizationSystem(&worldRasterizationSystem)
{
    _texture = SDL_CreateTexture(
        _renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_STREAMING,
        textureRect.width,
        textureRect.height);
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
    const SDL_Rect rect { _textureRect.x, _textureRect.y, _textureRect.width, _textureRect.height };
    if (SDL_RenderCopy(_renderer, _texture, nullptr, &rect)) {
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
        static_cast<int64_t>(pitch) * _textureRect.height,
        pitch,
        _texturePixelFormat
    };
    _rasterizationSystem->SetDestination(data);

    // TODO: extract render system, split render processing from logic processing:
    // 1. prepare render: add components with texture's part for each cell
    // 2. render: update texture
    // 3. post render: upload texture to driver
    _world->Update(elapsedTime);

    _rasterizationSystem->ResetDestination();
    SDL_UnlockTexture(_texture);
}
