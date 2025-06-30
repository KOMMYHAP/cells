#include "generated/auto_world_rasterization_lock_system.h"

#include "widgets/world/world_rasterization_target.h"

void WorldRasterizationLockSystem::DoSystemUpdate()
{
    _worldRasterizationTarget->Lock();
}