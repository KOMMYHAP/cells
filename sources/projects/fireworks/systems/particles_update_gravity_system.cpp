#include "generated/auto_particles_update_gravity_system.h"

#include "components/generated/auto_particle_position.h"
#include "components/generated/auto_particle_velocity.h"
#include "game_controller.h"

void UpdateParticleGravitySystem::DoProcessComponents(EcsEntity /*id*/, const ParticlePosition& particlePosition, ParticleVelocity& particleVelocity)
{
    const CellPosition gravityCenter = _gameController->GetGravityCenter();
    const float frameTimeSec = _gameController->GetFrameTime().AsSeconds();
    const float vectorX = static_cast<float>(gravityCenter.x) - particlePosition.x;
    const float vectorY = static_cast<float>(gravityCenter.y) - particlePosition.y;
    const float lengthSquare = vectorX * vectorX + vectorY * vectorY;
    const float gravityConstant = _gameController->GetGravityConstant();
    const float acceleration = gravityConstant / lengthSquare;
    const float accelerationPerFrame = acceleration * frameTimeSec;
    particleVelocity.value += accelerationPerFrame;
}