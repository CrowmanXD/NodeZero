#pragma once

#include <list>
#include <memory>

#include "IEvent.h"
#include "IObserver.h"
#include "ISubject.h"

/**
 * @class Subject
 * @brief Concrete implementation of the ISubject interface.
 *
 * Maintains a thread-unsafe list of observers.
 * Note: Notify() is safe against observers detaching themselves during the update loop.
 */
class Subject : public ISubject {
public:
    Subject() = default;
    virtual ~Subject() = default;

    void Attach(std::shared_ptr<IObserver> observer) override;
    void Detach(std::shared_ptr<IObserver> observer) override;
    void Notify(const std::shared_ptr<IEvent>& event) override;

private:
    /** @brief List used for efficient insertion/removal of observers. */
    std::list<std::shared_ptr<IObserver>> m_observers;
};