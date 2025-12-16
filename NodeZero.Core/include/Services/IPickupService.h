#pragma once

#include <vector>

#include "../Types/PointPickup.h"
#include "../Types/Position.h"


/**
 * @class IPickupService
 * @brief Interface for managing collectible items (pickups) in the game world.
 *
 * Handles spawning new pickups at specific locations, tracking their lifetimes,
 * and detecting when the player collects them.
 */
class IPickupService {
   public:
    virtual ~IPickupService() = default;

    /**
     * @brief Spawns a cluster of pickups around a specific origin point.
     * @param origin The center position to spawn around.
     * @param count The number of pickups to generate.
     * @param pointValue The score value of each pickup.
     */
    virtual void SpawnPointPickups(const Position& origin, int count, int pointValue) = 0;

    /**
     * @brief Attempts to collect a specific pickup by ID.
     * @param pickupId The unique ID of the pickup to collect.
     * @return True if the pickup existed and was collected, False otherwise.
     */
    virtual bool CollectPickup(int pickupId) = 0;

    /**
     * @brief Updates the lifetime of all active pickups and removes expired ones.
     * @param deltaTime Time elapsed since the last frame.
     */
    virtual void Update(float deltaTime) = 0;

    /**
     * @brief Removes all pickups and resets counters.
     */
    virtual void Clear() = 0;

    /**
     * @brief Gets a read-only list of all currently active pickups.
     */
    virtual const std::vector<PointPickup>& GetPickups() const = 0;

    /**
     * @brief Gets the total points accumulated from collected pickups.
     */
    virtual int GetPickupPoints() const = 0;

    /**
     * @brief Checks for pickups within a specific area and collects them.
     * * @param mouseX The X coordinate of the collection center (e.g., mouse cursor).
     * @param mouseY The Y coordinate of the collection center.
     * @param damageZoneSize The width/height of the collection area.
     * @param collectedPickups Output vector that will be filled with the collected items.
     */
    virtual void ProcessPickupCollection(float mouseX, float mouseY, float damageZoneSize,
                                         std::vector<PointPickup>& collectedPickups) = 0;
};
