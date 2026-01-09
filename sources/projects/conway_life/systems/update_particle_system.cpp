#include "SDL3/SDL_pixels.h"
#include "components/generated/auto_cell_emitter.h"
#include "components/generated/auto_cell_particle.h"
#include "generated/auto_update_particle_system.h"
#include "sdl_utils.h"

static SDL_Color LerpSdlColor(float t, SDL_Color from, SDL_Color to)
{
    SDL_Color color;
    color.r = std::clamp<uint8_t>(static_cast<uint8_t>(std::lerp(static_cast<float>(from.r), static_cast<float>(to.r), t)), 0, 255);
    color.g = std::clamp<uint8_t>(static_cast<uint8_t>(std::lerp(static_cast<float>(from.g), static_cast<float>(to.g), t)), 0, 255);
    color.b = std::clamp<uint8_t>(static_cast<uint8_t>(std::lerp(static_cast<float>(from.b), static_cast<float>(to.b), t)), 0, 255);
    color.a = std::clamp<uint8_t>(static_cast<uint8_t>(std::lerp(static_cast<float>(from.a), static_cast<float>(to.a), t)), 0, 255);
    return color;
}

void UpdateParticleSystem::DoProcessComponents(EcsEntity id, const CellEmitter& cellEmitter, CellParticle& cellParticle)
{
    if (cellParticle.lifetime <= 0) {
        _ecsWorld->remove<CellParticle>(id);
        return;
    }

    const float t = static_cast<float>(cellEmitter.particle_lifetime - cellParticle.lifetime) / static_cast<float>(cellEmitter.particle_lifetime);
    const SDL_Color colorFrom = UnpackColor(cellEmitter.color_from);
    const SDL_Color colorTo = UnpackColor(cellEmitter.color_to);
    const SDL_Color newColor = LerpSdlColor(t, colorFrom, colorTo);
    cellParticle.color = PackColor(newColor);
    cellParticle.lifetime -= 1;
}