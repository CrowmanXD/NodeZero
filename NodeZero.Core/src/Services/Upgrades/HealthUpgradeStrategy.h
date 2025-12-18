#pragma once

#include "UpgradeStrategy.h"
#include "Config/GameConfig.h"

/**
 * @class HealthUpgradeStrategy
 * @brief Strategy for increasing the player's maximum health capacity.
 *
 * Each successful application increases the base health pool by a fixed increment.
 */
class HealthUpgradeStrategy : public UpgradeStrategy {
public:
    /** * @brief Gets the cost of the health upgrade.
     * @return The point cost defined in GameConfig.
     */
    int GetCost() const override {
        return GameConfig::HEALTH_UPGRADE_COST;
    }

    /** * @brief Adds a fixed amount (1.0) to the player's maximum health.
     * @param data The save data structure to modify.
     */
    void Apply(SaveData& data) override {
        data.maxHealth += 1.0f;
    }
};