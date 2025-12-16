#pragma once

#include "raylib.h"

/**
 * @class InputHandler
 * @brief Static wrapper around Raylib's input system.
 *
 * Provides a unified interface for polling mouse and keyboard state.
 * This abstraction allows for easier mocking during unit testing (if needed)
 * or swapping the underlying input engine later.
 */
class InputHandler {
public:
    /** @brief Gets the current mouse cursor position in screen coordinates. */
    static Vector2 GetMousePosition();

    /** @brief Checks if a mouse button was pressed in this frame. */
    static bool IsMouseButtonPressed(int button);

    /** @brief Checks if a keyboard key was pressed in this frame (single trigger). */
    static bool IsKeyPressed(int key);

    /** @brief Checks if a keyboard key is currently held down. */
    static bool IsKeyDown(int key);
};