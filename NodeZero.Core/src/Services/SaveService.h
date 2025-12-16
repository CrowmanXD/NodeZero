#pragma once

#include <string>

#include "Services/ISaveService.h"
#include "Types/SaveData.h"

/**
 * @class SaveService
 * @brief Concrete implementation of the save system using a simple text-based format.
 *
 * This service handles file I/O, platform-specific path resolution (AppData on Windows,
 * Home on Linux), and serialization of the SaveData structure.
 */
class SaveService : public ISaveService {
private:
    SaveData m_CurrentData;

	//Refactor: No magic strings, turned into constants
    static constexpr const char* SAVE_FILE_NAME = "save.dat";
    static constexpr const char* KEY_HIGH_POINTS = "highPoints";
    static constexpr const char* KEY_POINTS = "points";
    static constexpr const char* KEY_GAMES_PLAYED = "gamesPlayed";
    static constexpr const char* KEY_NODES_DESTROYED = "totalNodesDestroyed";
    static constexpr const char* KEY_LEVEL = "currentLevel";
    static constexpr const char* KEY_MAX_HEALTH = "maxHealth";
    static constexpr const char* KEY_REGEN_RATE = "regenRate";
    static constexpr const char* KEY_ZONE_SIZE = "damageZoneSize";
    static constexpr const char* KEY_DAMAGE_TICK = "damagePerTick";

public:
    SaveService();
    ~SaveService() override = default;

    /**
     * @brief Initializes the service by attempting to load existing data.
     */
    void Initialize();

    SaveData LoadProgress() override;
    void SaveProgress(const SaveData& data) override;

    int GetPoints() const override;
    int GetHighPoints() const override;
    SaveData GetCurrentData() const override;

private:
    /**
     * @brief Determines the correct file path for saving data based on the OS.
     * @return The full absolute path to the save file.
     */
    std::string GetSavePath() const;

    /**
     * @brief Internal helper to write data to the disk.
     */
    bool WriteToFile(const SaveData& data) const;

    /**
     * @brief Internal helper to read and parse data from the disk.
     */
    SaveData ReadFromFile() const;
};