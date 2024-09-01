#pragma once

struct UiLayout {
    uint16_t screenWidth { 0 };
    uint16_t screenHeight { 0 };

    uint16_t fieldOffset { 0 };
    uint16_t fieldWidth { 0 };
    uint16_t fieldHeight { 0 };

    uint16_t cellPadding { 0 };
    uint8_t cellSize { 0 };
};