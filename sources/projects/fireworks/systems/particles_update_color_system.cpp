#include "sdl_utils.h"
#include "generated/auto_particles_update_color_system.h"

#include "SDL3/SDL_pixels.h"
#include "components/generated/auto_particle_color.h"
#include "components/generated/auto_particle_lifetime.h"

static SDL_Color LerpSdlColor(float t, SDL_Color from, SDL_Color to)
{
    SDL_Color color;
    color.r = std::clamp<uint8_t>(static_cast<uint8_t>(std::lerp(static_cast<float>(from.r), static_cast<float>(to.r), t)), 0, 255);
    color.g = std::clamp<uint8_t>(static_cast<uint8_t>(std::lerp(static_cast<float>(from.g), static_cast<float>(to.g), t)), 0, 255);
    color.b = std::clamp<uint8_t>(static_cast<uint8_t>(std::lerp(static_cast<float>(from.b), static_cast<float>(to.b), t)), 0, 255);
    color.a = std::clamp<uint8_t>(static_cast<uint8_t>(std::lerp(static_cast<float>(from.a), static_cast<float>(to.a), t)), 0, 255);
    return color;
}

void ParticlesUpdateColorSystem::DoProcessComponents(EcsEntity /*id*/, const ParticleLifetime& particleLifetime, ParticleColor& particleColor)
{
    const float t = particleLifetime.key;
    SDL_Color color = LerpSdlColor(t, UnpackColor(particleColor.startColor), UnpackColor(particleColor.endColor));
    particleColor.color = PackColor(color);
}