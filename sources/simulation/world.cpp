#include "world.h"

World::World(uint32_t width, uint32_t height)
    : _width(width)
    , _height(height)
    , brainSystem(width * height)
    , typeSystem(width * height)
    , virtualMachine(brainSystem, typeSystem)
    , positionSystem(width, height)
    , idSystem(width * height)
    , cellFactory(virtualMachine, brainSystem)
{
    virtualMachine.CreateProcedures(*this);
}
