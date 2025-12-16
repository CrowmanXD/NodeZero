#pragma once

/**
 * @struct GameConfig
 * @brief Central configuration container for game balance constants.
 *
 * Defines spawn rates, upgrade costs, damage values, and other gameplay parameters.
 * Modify these values to tune the game's difficulty and pacing.
 */
struct GameConfig {
    // --- Node Settings ---

    /** @brief Base movement speed of enemy nodes (Pixels per Second). */
    static constexpr float NODE_DEFAULT_SPEED = 75.0f;

    // --- Pickup Settings ---

    /** @brief How long a point pickup remains on screen before vanishing (Seconds). */
    static constexpr float PICKUP_LIFETIME = 10.0f;

    /** @brief Minimum age of a pickup before it can be collected (Seconds).
     * Prevents accidental collection immediately upon spawn. */
    static constexpr float PICKUP_COLLECT_DELAY = 0.1f;

    // --- Scoring Settings ---

    /** @brief Maximum combo multiplier for score calculation. */
    static constexpr int POINTS_MULTIPLIER_MAX = 5;

    // --- Player Health Settings ---

    /** @brief Starting maximum health for the player. */
    static constexpr float HEALTH_DEFAULT = 10.0f;

    /** @brief Cost in points to purchase a +1 Max Health upgrade. */
    static constexpr int HEALTH_UPGRADE_COST = 50;

    /** @brief Cost in points to purchase a regeneration upgrade. */
    static constexpr int REGEN_UPGRADE_COST = 100;

    /** @brief Amount of health restored per second per upgrade level. */
    static constexpr float REGEN_UPGRADE_AMOUNT = 0.1f;

    // --- Damage Zone Settings ---

    /** @brief Initial radius of the player's damage zone (Pixels). */
    static constexpr float DAMAGE_ZONE_DEFAULT_SIZE = 70.0f;

    /** @brief Cost to upgrade the size of the damage zone. */
    static constexpr int DAMAGE_ZONE_UPGRADE_COST = 75;

    /** @brief Radius added to the zone per upgrade level (Pixels). */
    static constexpr float DAMAGE_ZONE_UPGRADE_AMOUNT = 10.0f;

    /** @brief Hard cap for the damage zone radius (Pixels). */
    static constexpr float DAMAGE_ZONE_MAX_SIZE = 300.0f;

    /** @brief Base damage dealt to enemies per hit/tick. */
    static constexpr float DAMAGE_PER_TICK_DEFAULT = 50.0f;

    /** @brief Cost to increase damage per tick. */
    static constexpr int DAMAGE_UPGRADE_COST = 60;

    /** @brief Additional damage added per upgrade level. */
    static constexpr float DAMAGE_UPGRADE_AMOUNT = 5.0f;

    // --- Boss and Level Settings ---

    /** @brief Movement speed of the Boss node (Pixels per Second). */
    static constexpr float BOSS_SPEED = 35.0f;

    /** @brief Starting health pool for the Boss. */
    static constexpr float BOSS_HP_BASE = 200.0f;

    /** @brief Time required to survive to complete a level (Seconds). */
    static constexpr float LEVEL_DURATION = 60.0f;
};