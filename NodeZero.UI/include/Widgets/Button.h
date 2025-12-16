#pragma once

#include "IWidget.h"
#include "raylib.h"
#include <string>
#include <functional>

/**
 * @class Button
 * @brief An interactive UI component that triggers an action on click.
 *
 * Supports three visual states: Normal, Hovered, and Pressed.
 */
class Button : public IWidget
{
private:
    float m_X;
    float m_Y;
    float m_Width;
    float m_Height;
    std::string m_Text;
    Color m_NormalColor;
    Color m_HoverColor;
    Color m_PressedColor;
    Color m_TextColor;
    bool m_IsActive;
    std::function<void()> m_OnClick;
    Font m_Font;

	// Refactor: No magic number, turned into constant
    static constexpr float TEXT_SIZE_RATIO = 0.4f;

public:
    /**
     * @brief Constructs a Button.
     * @param x Top-left X coordinate.
     * @param y Top-left Y coordinate.
     * @param width Button width.
     * @param height Button height.
     * @param text The label text.
     * @param font The raylib Font resource.
     */
    Button(float x, float y, float width, float height, const char* text, Font font);

    void Draw() override;
    void Update() override;
    bool IsHovered() const override;
    bool IsActive() const override;
    void SetActive(bool active) override;

    /** @brief Sets the function to be executed when the button is clicked. */
    void SetOnClick(std::function<void()> callback);

    /**
     * @brief Sets the background colors for different states.
     * @param normal Color when idle.
     * @param hover Color when mouse is over the button.
     * @param pressed Color when the mouse button is down over the button.
     * @param text Color of the button text.
     */
    void SetColors(Color normal, Color hover, Color pressed, Color text);
};