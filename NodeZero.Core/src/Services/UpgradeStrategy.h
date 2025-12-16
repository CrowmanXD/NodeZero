#pragma once

struct SaveData;

class UpgradeStrategy {
public:
    virtual ~UpgradeStrategy() = default;

    virtual int GetCost() const = 0;
    virtual bool CanApply() const { return true; }
    virtual void Apply(SaveData& data) = 0;
};
