#pragma once

#include <functional>
#include "Services/IUpgradeService.h"
#include "Services/UpgradeStrategy.h"

class ISaveService;
struct SaveData;

/**
 * @class UpgradeService
 * @brief Concrete implementation of player upgrades using the Strategy Pattern.
 *
 * This service manages the purchase flow, validating funds and persisting
 * changes through the SaveService.
 */
class UpgradeService : public IUpgradeService {
private:
    float m_MaxHealth;
    float m_RegenRate;
    float m_DamageZoneSize;
    float m_DamagePerTick;
    ISaveService* m_SaveService;

    /** @brief Amount of Max HP added per upgrade level. */
    static constexpr float HEALTH_INCREMENT = 1.0f;

public:
    UpgradeService();
    ~UpgradeService() override = default;

    void Initialize(float maxHealth, float regenRate, float damageZoneSize, float damagePerTick) override;
    void SetSaveService(ISaveService* saveService);

    bool BuyHealthUpgrade() override;
    int GetHealthUpgradeCost() const override;
    float GetMaxHealth() const override;

    bool BuyRegenUpgrade() override;
    int GetRegenUpgradeCost() const override;
    float GetRegenRate() const override;

    bool BuyDamageZoneUpgrade() override;
    int GetDamageZoneUpgradeCost() const override;
    float GetDamageZoneSize() const override;

    bool BuyDamageUpgrade() override;
    int GetDamageUpgradeCost() const override;
    float GetDamagePerTick() const override;

private:
    /**
     * @brief Executes the purchase transaction using a specific strategy.
     * @param strategy The concrete upgrade strategy to execute.
     * @return True if points were deducted and stats applied successfully.
     */
    bool AttemptPurchase(UpgradeStrategy& strategy);
};