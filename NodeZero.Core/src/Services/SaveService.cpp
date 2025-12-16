#include "SaveService.h"

#include <algorithm>
#include <fstream>
#include <sstream>
#include <iostream>

#ifdef _WIN32
#include <shlobj.h>
#include <windows.h>
#else
#include <pwd.h>
#include <sys/types.h>
#include <unistd.h>
#endif

SaveService::SaveService() {
    m_CurrentData = ReadFromFile();
}

void SaveService::Initialize() {
    m_CurrentData = ReadFromFile();
}

SaveData SaveService::LoadProgress() {
    m_CurrentData = ReadFromFile();
    return m_CurrentData;
}

void SaveService::SaveProgress(const SaveData& data) {
    m_CurrentData = data;
    WriteToFile(data);
}

int SaveService::GetPoints() const {
    return m_CurrentData.points;
}

int SaveService::GetHighPoints() const {
    return m_CurrentData.highPoints;
}

SaveData SaveService::GetCurrentData() const {
    return m_CurrentData;
}

// Private Helper Methods (Refactored from static functions)

std::string SaveService::GetSavePath() const {
    std::string savePath;

#ifdef _WIN32
    // Windows: Use %APPDATA%/NodeZero/
    char appDataPath[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_APPDATA, NULL, 0, appDataPath))) {
        savePath = std::string(appDataPath) + "\\NodeZero\\";
        CreateDirectoryA(savePath.c_str(), NULL);
    }
    else {
        // Fallback to local directory if AppData fails
        savePath = "./";
    }
#else
    // Linux/Unix: Use ~/.local/share/NodeZero/
    const char* homeDir = getenv("HOME");
    if (homeDir == nullptr) {
        // Fallback: try to get user info from password database
        struct passwd* pw = getpwuid(getuid());
        if (pw) {
            homeDir = pw->pw_dir;
        }
    }

    if (homeDir != nullptr) {
        savePath = std::string(homeDir) + "/.local/share/NodeZero/";
        std::string mkdirCmd = "mkdir -p " + savePath;
        int result = system(mkdirCmd.c_str());
        (void)result;
    }
    else {
        savePath = "./";
    }
#endif

    return savePath + SAVE_FILE_NAME;
}

bool SaveService::WriteToFile(const SaveData& data) const {
    std::string filePath = GetSavePath();
    std::ofstream file(filePath);

    if (!file.is_open()) {
        return false;
    }

    // Refactor: Use named constants for JSON keys
    file << "{\n";
    file << "  \"" << KEY_HIGH_POINTS << "\": " << data.highPoints << ",\n";
    file << "  \"" << KEY_POINTS << "\": " << data.points << ",\n";
    file << "  \"" << KEY_GAMES_PLAYED << "\": " << data.gamesPlayed << ",\n";
    file << "  \"" << KEY_NODES_DESTROYED << "\": " << data.totalNodesDestroyed << ",\n";
    file << "  \"" << KEY_LEVEL << "\": " << data.currentLevel << ",\n";
    file << "  \"" << KEY_MAX_HEALTH << "\": " << data.maxHealth << ",\n";
    file << "  \"" << KEY_REGEN_RATE << "\": " << data.regenRate << ",\n";
    file << "  \"" << KEY_ZONE_SIZE << "\": " << data.damageZoneSize << ",\n";
    file << "  \"" << KEY_DAMAGE_TICK << "\": " << data.damagePerTick << "\n";
    file << "}\n";

    file.close();
    return true;
}

SaveData SaveService::ReadFromFile() const {
    SaveData data;
    std::string filePath = GetSavePath();
    std::ifstream file(filePath);

    if (!file.is_open()) {
        return data;
    }

    std::string line;
    while (std::getline(file, line)) {
        size_t colonPos = line.find(':');
        if (colonPos == std::string::npos) continue;

        std::string key = line.substr(0, colonPos);
        std::string value = line.substr(colonPos + 1);

        key.erase(std::remove(key.begin(), key.end(), '\"'), key.end());
        key.erase(std::remove(key.begin(), key.end(), ' '), key.end());
        value.erase(std::remove(value.begin(), value.end(), ','), value.end());
        value.erase(std::remove(value.begin(), value.end(), ' '), value.end());

        try {
            // Refactor: Use named constants for parsing
            if (key == KEY_HIGH_POINTS)      data.highPoints = std::stoi(value);
            else if (key == KEY_POINTS)      data.points = std::stoi(value);
            else if (key == KEY_GAMES_PLAYED) data.gamesPlayed = std::stoi(value);
            else if (key == KEY_NODES_DESTROYED) data.totalNodesDestroyed = std::stoi(value);
            else if (key == KEY_LEVEL)       data.currentLevel = std::stoi(value);
            else if (key == KEY_MAX_HEALTH)  data.maxHealth = std::stof(value);
            else if (key == KEY_REGEN_RATE)  data.regenRate = std::stof(value);
            else if (key == KEY_ZONE_SIZE)   data.damageZoneSize = std::stof(value);
            else if (key == KEY_DAMAGE_TICK) data.damagePerTick = std::stof(value);
        }
        catch (...) {
            continue;
        }
    }

    file.close();
    return data;
}