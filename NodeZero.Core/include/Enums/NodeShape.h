#pragma once

/**
 * @enum NodeShape
 * @brief Defines the visual shape and type of a node (enemy).
 *
 * Different shapes are visually distinct and can imply different collision
 * boundaries or behavior logic.
 */
enum class NodeShape {
    /** @brief Standard circular enemy. */
    Circle,

    /** @brief Standard square enemy. */
    Square,

    /** @brief Standard hexagonal enemy. */
    Hexagon,

    /** @brief Unique identifier for the Level Boss. */
    Boss
};