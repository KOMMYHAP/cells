#include "game_config.h"

const GameConfig::FireworksConfig* GameConfig::FindFireworks(int32_t epoch) const
{
    if (epoch < 0 || epoch >= _fireworks.size()) {
        return nullptr;
    }
    return &_fireworks[epoch];
}