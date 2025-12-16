#pragma once

#include "Enums/NodeShape.h"
#include "Services/ISpawnService.h"

/**
 * @class SpawnService
 * @brief Concrete implementation of the spawning logic.
 *
 * Implements edge-spawning logic (enemies appear outside the screen bounds)
 * and handles the difficulty curve by reducing spawn intervals as the level increases.
 */
class SpawnService : public ISpawnService {
   private:
    float m_ScreenWidth;
    float m_ScreenHeight;
    float m_SpawnTimer;
    int m_CurrentLevel;

    //Refactor: Game balance tweaks

    // Spawning Geometry
    static constexpr float SPAWN_EDGE_OFFSET = 50.0f;     // Distance outside screen to spawn
    static constexpr float TARGET_CENTER_VARIANCE = 150.0f; // Random aim offset from center

    // Difficulty Scaling
    static constexpr float HP_SCALING_FACTOR = 0.2f;      // 20% HP increase per level
    static constexpr float MIN_SPAWN_INTERVAL = 0.5f;     // Cap: Fastest spawn rate (0.5s)
    static constexpr float BASE_SPAWN_INTERVAL = 2.0f;    // Starting spawn rate (2.0s)
    static constexpr float INTERVAL_DECAY_RATE = 0.15f;   // Seconds removed per level

    // Shape Probabilities (Must sum to < 100 for the last case)
    static constexpr int CHANCE_SQUARE = 60;              // 0-59 (60%)
    static constexpr int CHANCE_CIRCLE = 90;              // 60-89 (30%)
    // Remainder is Hexagon (10%)

   public:
    SpawnService();
    ~SpawnService() override = default;

    /**
     * @brief Initializes screen dimensions used for edge calculations.
     */
    void Initialize(float screenWidth, float screenHeight);
    void UpdateAutoSpawn(float deltaTime) override;

    /**
     * @brief Resets the spawn timer to zero (e.g., at level start).
     */
    void ResetSpawnTimer();

    void SetCurrentLevel(int level);

    SpawnInfo GetNextSpawn() const override;
    float CalculateNodeHP(float baseHP) const override;
    bool ShouldAutoSpawn() const override;

   private:
    float RandomRange(float minValue, float maxValue) const;

    /**
     * @brief Selects a random shape based on defined probability weights.
     */
    NodeShape GetRandomShape() const;
};
