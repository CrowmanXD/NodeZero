#pragma once

#include <functional>
#include "Services/IUpgradeService.h"

class ISaveService;
struct SaveData;

/**
 * @class UpgradeService
 * @brief Concrete implementation of the upgrade logic.
 *
 * Interacts with the SaveService to persist upgrades immediately upon purchase.
 * Validates point balances and applies stat changes.
 */
class UpgradeService : public IUpgradeService {
private:
    float m_MaxHealth;
    float m_RegenRate;
    float m_DamageZoneSize;
    float m_DamagePerTick;
    ISaveService* m_SaveService;

	//Refactor: No magic numbers, turned into constant
    static constexpr float HEALTH_INCREMENT = 1.0f; // Fixed: Was hardcoded 1.0f

public:
    UpgradeService();
    ~UpgradeService() override = default;

    /**
     * @brief Initializes the service with values loaded from the save file.
     */
    void Initialize(float maxHealth, float regenRate, float damageZoneSize, float damagePerTick);

    /**
     * @brief Injects the SaveService dependency.
     */
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
     * @brief Refactoring Helper: generic transaction handler.
     * * Handles the common logic of checking for the service, validating funds,
     * deducting points, and saving the result.
     *
     * @param cost The cost of the upgrade.
     * @param applyEffect A lambda function that applies the specific stat change.
     * @return True if the transaction succeeded.
     */
    bool AttemptPurchase(int cost, const std::function<void(SaveData&)>& applyEffect);
};