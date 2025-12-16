#pragma once

#include "../Config/GameConfig.h"

/**
 * @struct SaveData
 * @brief Container for persistent player progression.
 *
 * This struct mirrors the JSON structure used in the SaveService.
 * It holds everything that needs to be remembered between game sessions.
 */
struct SaveData {
    // --- Progression Stats ---

    /** @brief The highest score ever achieved by the player. */
    int highPoints{ 0 };

    /** @brief Current currency available for spending in the shop. */
    int points{ 0 };

    /** @brief Total number of rounds/sessions played. */
    int gamesPlayed{ 0 };

    /** @brief Cumulative count of enemies destroyed across all games. */
    int totalNodesDestroyed{ 0 };

    /** @brief The level the player is currently on (affects difficulty). */
    int currentLevel{ 1 };

    // --- Upgradable Stats (Initialized with Config Defaults) ---

    /** @brief Player's maximum health cap. */
    float maxHealth{ GameConfig::HEALTH_DEFAULT };

    /** @brief Health regenerated per second. */
    float regenRate{ 0.0f };

    /** @brief Radius of the damage dealing area. */
    float damageZoneSize{ GameConfig::DAMAGE_ZONE_DEFAULT_SIZE };

    /** @brief Damage dealt per tick to enemies in the zone. */
    float damagePerTick{ GameConfig::DAMAGE_PER_TICK_DEFAULT };
};