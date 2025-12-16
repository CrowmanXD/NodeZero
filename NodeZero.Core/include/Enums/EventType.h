#pragma once

/**
 * @enum EventType
 * @brief Identifiers for the internal event messaging system.
 *
 * Used to decouple systems; for example, the Audio system can listen for
 * 'NodeDestroyed' to play a sound without needing access to the game logic code.
 */
enum class EventType {
    /** @brief Triggered when a new enemy node spawns. */
    NodeSpawned,

    /** @brief Triggered when a node takes damage. */
    NodeDamaged,

    /** @brief Triggered when a node's health reaches zero. */
    NodeDestroyed,

    /** @brief Triggered when the player's score changes. */
    PointsChanged,

    /** @brief Triggered when the combo multiplier changes. */
    MultiplierChanged,

    /** @brief Triggered when the game state changes. */
    GameStateChanged,

    /** @brief Triggered when the player loses (Health <= 0). */
    GameOver,

    /** @brief Triggered when the boss appears. */
    BossSpawned,

    /** @brief Triggered when the boss is defeated. */
    BossDefeated,

    /** @brief Triggered when the level timer expires successfully. */
    LevelCompleted
};