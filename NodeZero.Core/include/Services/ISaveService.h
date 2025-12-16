#pragma once

#include "Types/SaveData.h"


/**
 * @class ISaveService
 * @brief Interface for the data persistence system.
 *
 * Provides methods to save and load game progress (scores, levels, upgrades)
 * to permanent storage (disk).
 */
class ISaveService {
   public:
    virtual ~ISaveService() = default;

    /**
     * @brief Loads game progress from the disk.
     * @return A SaveData structure containing the loaded values, or defaults if no save exists.
     */
    virtual SaveData LoadProgress() = 0;

    /**
     * @brief Saves the current game progress to disk.
     * @param data The data structure containing values to serialize.
     */
    virtual void SaveProgress(const SaveData& data) = 0;

    /**
     * @brief Gets the current available currency/points from the cached data.
     */
    virtual int GetPoints() const = 0;

    /**
     * @brief Gets the all-time high score from the cached data.
     */
    virtual int GetHighPoints() const = 0;

    /**
    * @brief Returns the full cached save data object.
    */
    virtual SaveData GetCurrentData() const = 0;
};
