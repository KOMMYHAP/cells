#pragma once
#include "SDL3/SDL_pixels.h"
#include "ui_config.h"

template <
    int32_t CellsCountX, int32_t CellsCountY, int32_t CellPixelsSize,
    int32_t WindowSizeLimitX, int32_t WindowSizeLimitY,
    int32_t WorldWidgetOffsetX, int32_t WorldWidgetOffsetY>
struct GameUiConfig {
    static constexpr uint16_t WorldWidgetSizeX = CellsCountX * CellPixelsSize;
    static constexpr uint16_t WorldWidgetSizeY = CellsCountY * CellPixelsSize;

    static constexpr uint16_t CellsCountLimitX = (WindowSizeLimitX - 2 * WorldWidgetOffsetX) / CellPixelsSize;
    static_assert(CellsCountX <= CellsCountLimitX, "Maximum cells count by X reached!");

    static constexpr uint16_t CellsCountLimitY = (WindowSizeLimitY - 2 * WorldWidgetOffsetY) / CellPixelsSize;
    static_assert(CellsCountY <= CellsCountLimitY, "Maximum cells count by Y reached!");

    static constexpr uint16_t EstimatedWindowSizeX = WorldWidgetSizeX + 2 * WorldWidgetOffsetX;
    static constexpr uint16_t EstimatedWindowSizeY = WorldWidgetSizeY + 2 * WorldWidgetOffsetY;

    constexpr UiConfig ToUiConfig() const
    {
        return UiConfig {
            EstimatedWindowSizeX, EstimatedWindowSizeY,
            WorldWidgetSizeX, WorldWidgetSizeY,
            WorldWidgetOffsetX, WorldWidgetOffsetY,
            CellPixelsSize
        };
    }
};

class GameConfig {
public:
    UiConfig ui;

    SDL_Color emptyCellColor { 0, 0, 0, SDL_ALPHA_OPAQUE };

    int32_t emitterFramesToLive { 1000 };
    int32_t emitterFramesToEmit { 20 };

    int32_t particleFramesToLive { 100 };
    float particleVelocityX { 100.0f };
    float particleVelocityY { 100.0f };
    std::array<SDL_Color, 7> particleColors {
        SDL_Color { 0, 0, 255, SDL_ALPHA_OPAQUE },
        SDL_Color { 0, 255, 0, SDL_ALPHA_OPAQUE },
        SDL_Color { 255, 0, 0, SDL_ALPHA_OPAQUE },
        SDL_Color { 0, 255, 255, SDL_ALPHA_OPAQUE },
        SDL_Color { 255, 255, 0, SDL_ALPHA_OPAQUE },
        SDL_Color { 255, 0, 255, SDL_ALPHA_OPAQUE },
        SDL_Color { 255, 255, 255, SDL_ALPHA_OPAQUE },
    };
};