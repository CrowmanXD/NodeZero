#pragma once

/**
 * @struct Position
 * @brief Represents a 2D coordinate in the game world.
 *
 * Used for entity locations, spawn points, and vector math (direction).
 * The coordinate system origin (0,0) is typically the top-left of the screen.
 */
struct Position {
    /** @brief The X coordinate (Horizontal). */
    float x{ 0.0f };

    /** @brief The Y coordinate (Vertical). */
    float y{ 0.0f };

    /** @brief Default constructor initializes to (0,0). */
    Position() = default;

    /** @brief Constructs a position with specific coordinates. */
    Position(float x, float y) : x(x), y(y) {}
};