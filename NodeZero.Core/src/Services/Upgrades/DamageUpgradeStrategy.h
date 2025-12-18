#pragma once

#include "UpgradeStrategy.h"
#include "Config/GameConfig.h"

/**
 * @class DamageUpgradeStrategy
 * @brief Strategy for increasing the player's damage output.
 *
 * This strategy increases the damage dealt per tick to enemies within
 * the player's damage zone.
 */
class DamageUpgradeStrategy : public UpgradeStrategy {
public:
    /** * @brief Gets the cost of the damage upgrade from the game configuration.
     * @return The point cost defined in GameConfig.
     */
    int GetCost() const override {
        return GameConfig::DAMAGE_UPGRADE_COST;
    }

    /** * @brief Applies the damage increase to the persistent save data.
     * @param data The save data structure to modify.
     */
    void Apply(SaveData& data) override {
        data.damagePerTick += GameConfig::DAMAGE_UPGRADE_AMOUNT;
    }
};