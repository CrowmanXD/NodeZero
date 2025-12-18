#pragma once

#include "UpgradeStrategy.h"
#include "Config/GameConfig.h"

/**
 * @class DamageZoneUpgradeStrategy
 * @brief Strategy for increasing the radius of the player's damage zone.
 *
 * Unlike other upgrades, this strategy includes a cap check to ensure the
 * zone does not exceed the maximum allowed size.
 */
class DamageZoneUpgradeStrategy : public UpgradeStrategy {
public:
    /** * @brief Gets the cost of the zone size upgrade.
     * @return The point cost defined in GameConfig.
     */
    int GetCost() const override {
        return GameConfig::DAMAGE_ZONE_UPGRADE_COST;
    }

    /** * @brief Checks if the upgrade can be applied based on the size cap.
     * @return True if the current size is below the maximum allowed size.
     */
    bool CanApply() const override {
        return currentSize < GameConfig::DAMAGE_ZONE_MAX_SIZE;
    }

    /** * @brief Sets the current size context for the limit check.
     * @param size The current damage zone size from the service.
     */
    void SetCurrentSize(float size) {
        currentSize = size;
    }

    /** * @brief Increases the zone size in the save data, respecting the hard cap.
     * @param data The save data structure to modify.
     */
    void Apply(SaveData& data) override {
        currentSize += GameConfig::DAMAGE_ZONE_UPGRADE_AMOUNT;

        if (currentSize > GameConfig::DAMAGE_ZONE_MAX_SIZE) {
            currentSize = GameConfig::DAMAGE_ZONE_MAX_SIZE;
        }

        data.damageZoneSize = currentSize;
    }

private:
    /** @brief Internal tracking of size for the CanApply validation. */
    float currentSize = 0.0f;
};