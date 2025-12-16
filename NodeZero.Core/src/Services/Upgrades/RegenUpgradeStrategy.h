#pragma once

#include "UpgradeStrategy.h"
#include "Config/GameConfig.h"

class RegenUpgradeStrategy : public UpgradeStrategy {
public:
    int GetCost() const override {
        return GameConfig::REGEN_UPGRADE_COST;
    }

    void Apply(SaveData& data) override {
        data.regenRate += GameConfig::REGEN_UPGRADE_AMOUNT;
    }
};
