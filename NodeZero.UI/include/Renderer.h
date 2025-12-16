#pragma once

#include "raylib.h"

/**
 * @class Renderer
 * @brief Static utility class for all game rendering operations.
 *
 * Encapsulates the logic for drawing geometric shapes, UI elements,
 * and complex entity visuals (like the health-fill effect on nodes).
 * Relies on Raylib for low-level drawing calls.
 */
class Renderer {
public:
    // --- Entity Drawing ---

    /**
     * @brief Draws a circular node with a health-fill effect.
     * @param x Center X coordinate.
     * @param y Center Y coordinate.
     * @param size Radius of the circle.
     * @param hpPercentage Fill amount (0.0 to 1.0).
     * @param color Base color of the entity.
     * @param rotation Rotation in degrees.
     */
    static void DrawCircleNode(float x, float y, float size, float hpPercentage, Color color, float rotation = 0.0f);

    /** @brief Draws a square node with health-fill effect. */
    static void DrawSquareNode(float x, float y, float size, float hpPercentage, Color color, float rotation = 0.0f);

    /** @brief Draws a hexagonal node with health-fill effect. */
    static void DrawHexagonNode(float x, float y, float size, float hpPercentage, Color color, float rotation = 0.0f);

    /** @brief Draws a simple cross/plus shape for pickups. */
    static void DrawPickup(float x, float y, float size, Color color);

    // --- UI Drawing ---

    /** @brief Draws FPS and debug counters. */
    static void DrawDebugInfo(int posX, int posY, Font font);

    /** @brief Draws the player's score counter. */
    static void DrawPoints(int points, int posX, int posY, int fontSize, Color color, Font font);

    /** @brief Draws a standard health bar with text overlay. */
    static void DrawHealthBar(float health, float maxHealth, int posX, int posY, int width, int height, Font font);

    /** @brief Draws the level progress bar at the bottom of the screen. */
    static void DrawProgressBar(float percentage, int currentLevel, Font font);
};