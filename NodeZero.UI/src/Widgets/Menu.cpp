#include "Widgets/Menu.h"

Menu::Menu()
    : m_IsActive(true) {
}

void Menu::Draw() {
    if (!m_IsActive)
        return;

    for (auto& widget : m_Widgets) {
        if (widget)
            widget->Draw();
    }
}

void Menu::Update() {
    if (!m_IsActive)
        return;

    for (auto& widget : m_Widgets) {
        if (widget)
            widget->Update();
    }
}

bool Menu::IsHovered() const {
    for (const auto& widget : m_Widgets) {
        if (widget && widget->IsActive() && widget->IsHovered())
            return true;
    }
    return false;
}

bool Menu::IsActive() const {
    return m_IsActive;
}

void Menu::SetActive(bool active) {
    m_IsActive = active;
}

void Menu::AddWidget(std::unique_ptr<IWidget> widget) {
    if (widget)
        m_Widgets.push_back(std::move(widget));
}

void Menu::Clear() {
    // Removes all unique_ptrs, destroying the managed widgets.
    m_Widgets.clear();
}