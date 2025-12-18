#pragma once

#include "UpgradeStrategy.h"
#include "Config/GameConfig.h"

/**
 * @class RegenUpgradeStrategy
 * @brief Strategy for increasing the player's passive health regeneration rate.
 */
class RegenUpgradeStrategy : public UpgradeStrategy {
public:
    /** * @brief Gets the cost of the regeneration upgrade.
     * @return The point cost defined in GameConfig.
     */
    int GetCost() const override {
        return GameConfig::REGEN_UPGRADE_COST;
    }

    /** * @brief Increases the regeneration rate in the save data.
     * @param data The save data structure to modify.
     */
    void Apply(SaveData& data) override {
        data.regenRate += GameConfig::REGEN_UPGRADE_AMOUNT;
    }
};