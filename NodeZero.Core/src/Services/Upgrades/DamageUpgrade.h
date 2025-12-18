#pragma once

#include "UpgradeStrategy.h"
#include "Config/GameConfig.h"

/**
 * @class DamageUpgradeStrategy
 * @brief Strategy for increasing the player's damage output.
 *
 * This concrete strategy handles the logic for purchasing a damage increase.
 * It interacts with the GameConfig to determine costs and increment values.
 */
class DamageUpgradeStrategy : public UpgradeStrategy {
public:
    /** * @brief Gets the point cost required to purchase this upgrade.
     * @return The cost defined in GameConfig::DAMAGE_UPGRADE_COST.
     */
    int GetCost() const override {
        return GameConfig::DAMAGE_UPGRADE_COST;
    }

    /** * @brief Applies the damage upgrade to the player's save data.
     * * Increases the `damagePerTick` stat by the configured amount.
     * @param data The persistent game data to be modified.
     */
    void Apply(SaveData& data) override {
        data.damagePerTick += GameConfig::DAMAGE_UPGRADE_AMOUNT;
    }
};