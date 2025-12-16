#pragma once

#include <memory>
#include <string>

/**
 * @class IEvent
 * @brief Base interface for all system events.
 *
 * Provides a common type that can be passed through the Event Bus (Subject).
 * Specific event data should be casted based on the type.
 */
class IEvent {
public:
    virtual ~IEvent() = default;

    /**
     * @brief Gets the string representation of the event type.
     * @return A string (e.g., "NodeSpawned"). Useful for logging/debugging.
     */
    virtual std::string GetType() const = 0;

    /**
     * @brief Gets the time the event occurred.
     * @return Timestamp in seconds.
     */
    virtual float GetTimestamp() const = 0;
};