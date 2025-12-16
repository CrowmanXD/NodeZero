#pragma once

#include <memory>

class IObserver;
class IEvent;

/**
 * @class ISubject
 * @brief Interface for the Subject (Publisher) in the Observer Pattern.
 *
 * Manages a list of observers and broadcasts events to them.
 */
class ISubject {
public:
    virtual ~ISubject() = default;

    /**
     * @brief Registers an observer to receive notifications.
     */
    virtual void Attach(std::shared_ptr<IObserver> observer) = 0;

    /**
     * @brief Unregisters an observer.
     */
    virtual void Detach(std::shared_ptr<IObserver> observer) = 0;

    /**
     * @brief Broadcasts an event to all attached observers.
     */
    virtual void Notify(const std::shared_ptr<IEvent>& event) = 0;
};