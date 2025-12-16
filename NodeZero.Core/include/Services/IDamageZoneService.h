#pragma once

#include <functional>
#include <vector>

class Node;

/**
 * @class IDamageZoneService
 * @brief Interface defining the contract for handling damage zones in the game.
 *
 * This service manages the timing and application of damage to nodes
 * that fall within specific rectangular areas.
 */
class IDamageZoneService {
   public:
    virtual ~IDamageZoneService() = default;

    /**
     * @brief Processes a damage zone, applying damage to all nodes within it.
     *
     * @param centerX The X coordinate of the zone's center.
     * @param centerY The Y coordinate of the zone's center.
     * @param zoneSize The width/height of the square damage zone.
     * @param damage The amount of damage to apply to affected nodes.
     * @param currentLevel The current game level (affects scaling costs).
     * @param nodes A vector of pointers to the nodes to check.
     * @param onNodeDamaged A callback function invoked when a node takes damage.
     * Provides the damaged node and the calculated health cost.
     */
    virtual void ProcessDamageZone(
        float centerX,
        float centerY,
        float zoneSize,
        float damage,
        int currentLevel,
        const std::vector<Node*>& nodes,
        std::function<void(Node*, float)> onNodeDamaged) = 0;

    /**
     * @brief Updates the internal cooldown timer for damage ticks.
     * @param deltaTime The time elapsed since the last frame (in seconds).
     */
    virtual void UpdateTimer(float deltaTime) = 0;

    /**
     * @brief Resets the damage cooldown timer to zero.
     */
    virtual void ResetTimer() = 0;

    /**
     * @brief Checks if enough time has passed to deal damage again.
     * @return True if the timer exceeds the damage interval, False otherwise.
     */
    virtual bool ShouldDealDamage() const = 0;
};
