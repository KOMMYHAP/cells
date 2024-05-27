#pragma once

struct Component {
    std::string_view name;
    uint8_t numbersCount { 0 };
    uint16_t sizeInBytes { 0 };
};