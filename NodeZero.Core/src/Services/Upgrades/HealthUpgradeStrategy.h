#pragma once

#include "UpgradeStrategy.h"
#include "Config/GameConfig.h"

class HealthUpgradeStrategy : public UpgradeStrategy {
public:
    int GetCost() const override {
        return GameConfig::HEALTH_UPGRADE_COST;
    }

    void Apply(SaveData& data) override {
        data.maxHealth += 1.0f;
    }
};
