#pragma once

#include <functional>
#include <memory>
#include <vector>

#include "Enums/GameScreen.h"
#include "Events/IObserver.h"
#include "IGame.h"
#include "raylib.h"

// Forward declaration
class INode;

/**
 * @struct PickupCollectEffect
 * @brief Tracks the animation state of a pickup moving towards the mouse cursor after collection.
 */
struct PickupCollectEffect {
    Vector2 startPosition;
    float elapsed;
    float duration;
    float size;
};

/**
 * @struct DamageParticle
 * @brief Tracks the state of a single particle spawned upon enemy damage (e.g., blood/gore).
 */
struct DamageParticle {
    Vector2 position;
    Vector2 velocity;
    float lifetime;
    float maxLifetime;
    float size;
    Color color;
};

/**
 * @class GameplayScreen
 * @brief The main active game screen where rendering and primary update logic occurs.
 *
 * Implements the IObserver interface to react to game events (e.g., NodeDamaged)
 * to trigger visual effects (screen shake, particles).
 */
class GameplayScreen : public IObserver, public std::enable_shared_from_this<GameplayScreen> {
public:
    GameplayScreen(IGame& game, std::function<void(GameScreen)> stateChangeCallback, Font font);

    void Update(float deltaTime);
    void Draw();

    /** @brief Clears all running visual effects, typically when transitioning out of the screen. */
    void ClearEffects();

    void Update(const std::shared_ptr<IEvent>& event) override;

private:
    IGame& m_Game;
    std::function<void(GameScreen)> m_StateChangeCallback;
    std::vector<PickupCollectEffect> m_PickupEffects;
    std::vector<DamageParticle> m_DamageParticles;
    Font m_Font;

    // Constants (Refactor: Visual/Physics Tuning)

    // Limits
    static constexpr size_t MAX_PARTICLES = 500;
    static constexpr size_t MAX_PICKUP_EFFECTS = 100;

    // Timings
    static constexpr float PICKUP_COLLECT_EFFECT_DURATION = 1.0f;
    static constexpr float PICKUP_SPAWN_ANIM_DURATION = 0.45f;
    static constexpr float PARTICLE_LIFETIME = 0.6f;
    static constexpr float SHAKE_DURATION = 0.15f;

    // Screen Shake
    static constexpr float SHAKE_INTENSITY = 8.0f;

    // Particles
    static constexpr int PARTICLE_COUNT = 8;
    static constexpr float PARTICLE_SPEED_MIN = 50.0f;
    static constexpr float PARTICLE_SPEED_MAX = 150.0f;
    static constexpr float PARTICLE_GRAVITY = 200.0f;
    static constexpr float PARTICLE_BASE_SIZE_SCALING = 3.0f;
    static constexpr int PARTICLE_COLOR_VARIANCE = 20;

    // Drawing/UI
    static constexpr float REFLECTION_OFFSET_RATIO = 0.02f;
    static constexpr float CORNER_LENGTH_RATIO = 0.02f;
    static constexpr float CORNER_THICKNESS_RATIO = 0.003f;
    static constexpr float CENTER_SQUARE_SIZE_RATIO = 0.008f;

    float m_ShakeIntensity{ 0.0f };
    float m_ShakeDuration{ 0.0f };
    float m_ShakeTimer{ 0.0f };
    Vector2 m_ShakeOffset{ 0.0f, 0.0f };

    void TriggerShake(float intensity, float duration);
    void UpdateShake(float deltaTime);
    void SpawnDamageParticles(Vector2 position, Color baseColor, int count);
    void UpdateParticles(float deltaTime);

    /** @brief Draws the offset shadows, contributing to the neon/reflection effect. */
    void DrawReflections(const std::vector<INode*>& nodes, Vector2 mousePos, float damageZoneSize, float reflectionOffset);

    /** @brief Draws the glowing circles behind entities, contributing to the neon/bloom effect. */
    void DrawBloom(const std::vector<INode*>& nodes, Vector2 mousePos, float damageZoneSize);
};