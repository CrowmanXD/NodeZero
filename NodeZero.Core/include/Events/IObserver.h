#pragma once

#include <memory>

class IEvent;

/**
 * @class IObserver
 * @brief Interface for objects that listen for events (Subscriber).
 *
 * Implement this interface and attach the object to a Subject to receive updates.
 */
class IObserver {
public:
    virtual ~IObserver() = default;

    /**
     * @brief Called by the Subject when an event occurs.
     * @param event The generic event data. Cast to specific types (e.g., GameEvent) to use.
     */
    virtual void Update(const std::shared_ptr<IEvent>& event) = 0;
};