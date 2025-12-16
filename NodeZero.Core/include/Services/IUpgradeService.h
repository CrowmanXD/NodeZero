#pragma once

/**
 * @class IUpgradeService
 * @brief Interface for the game's upgrade and progression system.
 *
 * Handles the logic for purchasing permanent player improvements using
 * currency (points). It manages Health, Regeneration, Area of Effect (Zone),
 * and Damage output upgrades.
 */
class IUpgradeService {
   public:
    virtual ~IUpgradeService() = default;

    /**
     * @brief Attempts to purchase a Maximum Health upgrade.
     * @return True if the purchase was successful, False if insufficient funds.
     */
    virtual bool BuyHealthUpgrade() = 0;

    /**
     * @brief Attempts to purchase a Health Regeneration upgrade.
     * @return True if successful, False otherwise.
     */
    virtual bool BuyRegenUpgrade() = 0;

    /**
     * @brief Attempts to purchase a Damage Zone size upgrade.
     * Capped at a maximum size.
     * @return True if successful, False if insufficient funds or max level reached.
     */
    virtual bool BuyDamageZoneUpgrade() = 0;

    /**
     * @brief Attempts to purchase a Damage Per Tick upgrade.
     * @return True if successful, False otherwise.
     */
    virtual bool BuyDamageUpgrade() = 0;

    /**
     * @brief Gets the current maximum health value including upgrades.
     */
    virtual float GetMaxHealth() const = 0;

    /**
     * @brief Gets the current regeneration rate per second.
     */
    virtual float GetRegenRate() const = 0;

    /**
     * @brief Gets the current size of the damage zone.
     */
    virtual float GetDamageZoneSize() const = 0;

    /**
     * @brief Gets the current damage dealt per tick.
     */
    virtual float GetDamagePerTick() const = 0;

    virtual int GetHealthUpgradeCost() const = 0;
    virtual int GetRegenUpgradeCost() const = 0;
    virtual int GetDamageZoneUpgradeCost() const = 0;
    virtual int GetDamageUpgradeCost() const = 0;
};
