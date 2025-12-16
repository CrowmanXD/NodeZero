#pragma once

#include "raylib.h"

/**
 * @class IWidget
 * @brief Base interface for all UI elements (widgets).
 *
 * Defines the essential contract for drawing, updating state, and managing
 * activation status for any interactive or static UI component.
 */
class IWidget {
public:
    virtual ~IWidget() = default;

    /** @brief Renders the widget to the screen. */
    virtual void Draw() = 0;

    /** @brief Handles input or logic updates for the widget. */
    virtual void Update() = 0;

    /** @brief Checks if the mouse cursor is currently over the widget area. */
    virtual bool IsHovered() const = 0;

    /** @brief Checks if the widget is enabled and should be drawn/updated. */
    virtual bool IsActive() const = 0;

    /** @brief Sets the active/enabled state of the widget. */
    virtual void SetActive(bool active) = 0;
};