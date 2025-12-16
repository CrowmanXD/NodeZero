#pragma once

#include "UpgradeStrategy.h"
#include "Config/GameConfig.h"


class DamageUpgradeStrategy : public UpgradeStrategy {
public:
    int GetCost() const override {
        return GameConfig::DAMAGE_UPGRADE_COST;
    }

    void Apply(SaveData& data) override {
        data.damagePerTick += GameConfig::DAMAGE_UPGRADE_AMOUNT;
    }
};
