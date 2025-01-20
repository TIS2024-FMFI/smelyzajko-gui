#pragma once

#include "GUI.h"
#include "TemplateManager.h"
#include <vector>
#include <string>
#include <map>
#include <chrono>
#include <filesystem>

using json = nlohmann::json;

class ReplayMode : public GUI {
public:
    ReplayMode(YAML::Node configFile);  // Constructor
    int run() override;                 // Main run method

    void loadLogData(const std::string& directoryPath);  // Load log data from directory

private:
    void drawMenuBar();                // Render the playback menu
    void drawElements();               // Render elements on the screen
    void setupShortcuts() override;    // Set up keyboard shortcuts
    void handlePlayback();             // Handle playback logic
    void processLogFile(const std::filesystem::path& filePath, const std::string& moduleName); // Process a single log file

    void play();                       // Start playback
    void pause();                      // Pause playback
    void stop();                       // Stop playback
    void nextFrame();                  // Move to the next frame
    void previousFrame();              // Move to the previous frame

    TemplateManager templateManager;   // Manages templates and elements
    std::vector<std::pair<float, json>> replayData;  // Replay data: <timestamp, data>
    size_t currentFrame = 0;           // Current playback frame
    bool isPlaying = false;            // Playback state
    float playbackSpeed = 1.0f;        // Playback speed
    std::chrono::time_point<std::chrono::steady_clock> lastUpdate; // Time of the last update
    std::string logDirectory;          // Path to the log directory
    std::vector<std::string> modules;  // List of modules to process

    static constexpr float MIN_PLAYBACK_SPEED = 0.1f;
    static constexpr float MAX_PLAYBACK_SPEED = 5.0f;
};
