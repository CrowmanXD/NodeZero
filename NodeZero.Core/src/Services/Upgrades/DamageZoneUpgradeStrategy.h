#pragma once

#include "UpgradeStrategy.h"
#include "Config/GameConfig.h"

class DamageZoneUpgradeStrategy : public UpgradeStrategy {
public:
    int GetCost() const override {
        return GameConfig::DAMAGE_ZONE_UPGRADE_COST;
    }

    bool CanApply() const override {
        return currentSize < GameConfig::DAMAGE_ZONE_MAX_SIZE;
    }

    void SetCurrentSize(float size) {
        currentSize = size;
    }

    void Apply(SaveData& data) override {
        currentSize += GameConfig::DAMAGE_ZONE_UPGRADE_AMOUNT;
        if (currentSize > GameConfig::DAMAGE_ZONE_MAX_SIZE) {
            currentSize = GameConfig::DAMAGE_ZONE_MAX_SIZE;
        }
        data.damageZoneSize = currentSize;
    }

private:
    float currentSize = 0.0f;
};
