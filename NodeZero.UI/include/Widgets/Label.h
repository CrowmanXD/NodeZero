#pragma once

#include "IWidget.h"
#include "raylib.h"
#include <string>

/**
 * @class Label
 * @brief A static, non-interactive UI component for displaying text.
 *
 * Implements IWidget but typically returns false for IsHovered().
 */
class Label : public IWidget
{
private:
    float m_X;
    float m_Y;
    std::string m_Text;
    int m_FontSize;
    Color m_Color;
    bool m_IsActive;
    Font m_Font;

public:
    /**
     * @brief Constructs a Label at a specific position.
     * @param x The starting X coordinate.
     * @param y The starting Y coordinate.
     * @param text The string content.
     * @param font The raylib Font resource to use.
     * @param fontSize The size of the text.
     * @param color The text color.
     */
    Label(float x, float y, const char* text, Font font, int fontSize = 20, Color color = BLACK);

    void Draw() override;
    void Update() override;
    bool IsHovered() const override;
    bool IsActive() const override;
    void SetActive(bool active) override;

    /** @brief Sets the text content of the label. */
    void SetText(const char* text);

    /** @brief Gets the text content of the label. */
    const char* GetText() const;
};