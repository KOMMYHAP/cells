#include "generated/auto_particles_update_gravity_system.h"

#include "components/generated/auto_particle_position.h"
#include "components/generated/auto_particle_velocity.h"
#include "game_controller.h"

void ParticlesUpdateGravitySystem::DoProcessComponents(EcsEntity /*id*/, const ParticlePosition& particlePosition, ParticleVelocity& particleVelocity)
{
    const CellPosition gravityCenter = _gameController->GetGravityCenter();
    const double frameTimeSec = _gameController->GetFrameTime().AsSeconds();
    const double vectorX = static_cast<double>(gravityCenter.x) - particlePosition.x;
    const double vectorY = static_cast<double>(gravityCenter.y) - particlePosition.y;
    const double lengthSquare = vectorX * vectorX + vectorY * vectorY;
    const double gravityConstant = _gameController->GetGravityConstant();
    const double acceleration = gravityConstant / lengthSquare;
    const double accelerationPerFrame = acceleration * frameTimeSec;
    particleVelocity.valueX += static_cast<float>(accelerationPerFrame);
    particleVelocity.valueY += static_cast<float>(accelerationPerFrame);
}