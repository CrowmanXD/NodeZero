#include "Events/Subject.h"

void Subject::Attach(std::shared_ptr<IObserver> observer) {
    if (!observer) {
        return;
    }
    m_observers.push_back(observer);
}

void Subject::Detach(std::shared_ptr<IObserver> observer) {
    if (!observer) {
        return;
    }
    m_observers.remove(observer);
}

void Subject::Notify(const std::shared_ptr<IEvent>& event) {
    if (!event) {
        return;
    }

    // Refactor Note: We create a copy of the list before iterating.
    // Why? If an Observer decides to call Detach() inside its Update() method,
    // iterating over the original 'm_observers' while modifying it would crash the program.
    auto observersCopy = m_observers;

    for (const auto& observer : observersCopy) {
        if (observer) {
            observer->Update(event);
        }
    }
}