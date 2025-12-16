#pragma once

#include "../Types/SpawnInfo.h"

/**
 * @class ISpawnService
 * @brief Interface for the enemy spawning system.
 *
 * Handles the logic for when and where enemies appear, including
 * position calculation (screen edges), trajectory targeting, and
 * difficulty scaling (HP calculations).
 */
class ISpawnService {
   public:
    virtual ~ISpawnService() = default;

    /**
     * @brief Updates the internal spawn timer.
     * @param deltaTime Time elapsed since the last frame (in seconds).
     */
    virtual void UpdateAutoSpawn(float deltaTime) = 0;

    /**
     * @brief Checks if the timer has exceeded the spawn interval.
     * @return True if it is time to spawn a new enemy, False otherwise.
     */
    virtual bool ShouldAutoSpawn() const = 0;

    /**
     * @brief Calculates all necessary data for a new enemy instance.
     * Generates a position on the screen edge and a direction vector.
     * @return A SpawnInfo struct containing position, shape, and velocity.
     */
    virtual SpawnInfo GetNextSpawn() const = 0;

    /**
     * @brief Calculates enemy health based on base stats and current level.
     * @param baseHP The standard health value for the enemy type.
     * @return The scaled health value.
     */
    virtual float CalculateNodeHP(float baseHP) const = 0;
};
