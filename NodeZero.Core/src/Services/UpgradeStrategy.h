#pragma once

struct SaveData;

/**
 * @class UpgradeStrategy
 * @brief Abstract base class for the Strategy Pattern used in player upgrades.
 *
 * Defines the contract for different types of upgrades (Health, Regen, etc.),
 * allowing the UpgradeService to process purchases generically.
 */
class UpgradeStrategy {
public:
    virtual ~UpgradeStrategy() = default;

    /** @brief Returns the point cost of the specific upgrade. */
    virtual int GetCost() const = 0;

    /** @brief Returns true if requirements for the upgrade are met. */
    virtual bool CanApply() const { return true; }

    /** @brief Applies the specific stat changes to the provided SaveData. */
    virtual void Apply(SaveData& data) = 0;
};