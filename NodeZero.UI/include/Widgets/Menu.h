#pragma once

#include "IWidget.h"
#include "Button.h"
#include "Label.h"
#include <vector>
#include <memory>

/**
 * @class Menu
 * @brief A composite UI widget that acts as a container for other IWidget objects.
 *
 * Manages a collection of buttons, labels, or other widgets and forwards
 * Draw and Update calls to its children.
 */
class Menu : public IWidget
{
private:
    std::vector<std::unique_ptr<IWidget>> m_Widgets;
    bool m_IsActive;

public:
    Menu();

    void Draw() override;
    void Update() override;
    bool IsHovered() const override;
    bool IsActive() const override;
    void SetActive(bool active) override;

    /**
     * @brief Adds a child widget to the menu container.
     * @param widget A unique pointer to the widget (Menu takes ownership).
     */
    void AddWidget(std::unique_ptr<IWidget> widget);

    /** @brief Removes all widgets from the menu. */
    void Clear();
};