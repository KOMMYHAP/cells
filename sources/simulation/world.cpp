#include "world.h"

World::World(uint32_t width, uint32_t height)
    : brainSystem(width * height)
    , positionSystem(width, height)
    , idSystem(width * height)
    , typeSystem(width * height)
    , _width(width)
    , _height(height)
{
}