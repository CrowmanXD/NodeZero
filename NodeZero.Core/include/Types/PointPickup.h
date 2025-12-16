#pragma once

#include "Position.h"

/**
 * @struct PointPickup
 * @brief Represents a collectible item dropped by destroyed enemies.
 *
 * Contains logic to track its own lifetime and fading status.
 */
struct PointPickup
{
    /** @brief Unique identifier for collection logic. */
    int id{ 0 };

    /** @brief Current world position. */
    Position position;

    /** @brief The center point where this cluster of pickups spawned. */
    Position spawnOrigin;

    /** @brief Visual radius of the pickup. */
    float size{ 0.0f };

    /** @brief Total duration this pickup lasts before despawning (Seconds). */
    float lifetime{ 0.0f };

    /** @brief Current time left before despawn (Seconds). */
    float remainingTime{ 0.0f };

    /** @brief Value added to score/currency when collected. */
    int points{ 0 };

    /**
     * @brief Calculates the percentage of life remaining.
     * @return A value between 0.0 (expired) and 1.0 (fresh).
     * Useful for fading animations (alpha).
     */
    [[nodiscard]] float GetLifeRatio() const
    {
        if (lifetime <= 0.0f)
        {
            return 0.0f;
        }
        return remainingTime / lifetime;
    }

    /**
     * @brief Calculates how long the pickup has been alive.
     * @return Time elapsed since spawn (Seconds).
     */
    [[nodiscard]] float GetAge() const
    {
        return lifetime - remainingTime;
    }
};