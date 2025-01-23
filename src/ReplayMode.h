#pragma once
#include "GUI.h"
#include "TemplateManager.h"
#include <vector>
#include <string>
#include <map>
#include <chrono>
#include <filesystem>
#include <condition_variable>



class ReplayMode : public GUI {
public:
      ReplayMode(YAML::Node configFile): GUI(configFile) {
            std::vector<std::string> templateNames;

            if (configFile["templates"]) {
                for (const auto& templateNode : configFile["templates"]) {
                    std::string templateName = templateNode.as<std::string>();
                    templateNames.push_back(templateName);
                }
            } else {
                std::cerr << "No templates found in config file." << std::endl;
            }

            if (templateNames.empty()) {
                templateManager = TemplateManager(false);
            } else {
                templateManager = TemplateManager(templateNames, false);
            }
            setupShortcuts() ;

            templateManager.setActiveTemplate(templateManager.getAllTemplates().front());
            std::string activeTemplateName = templateManager.getActiveTemplateName();
            std::string windowTitle = std::string("GUI") + " - " + activeTemplateName;
            glfwSetWindowTitle(window, windowTitle.c_str());
    };
    int run() override;
    void setupShortcuts() override;
    void drawMenuBar();
    TemplateManager templateManager;
    void startGraphicModuleThreads();
    void stopGraphicModuleThreads();
    void loadLogData();
    void back();



private:
    int currentTemplateIndex = 0;
    std::string logDirectory;
    void runGraphicModule(GraphicModule* module);
    std::atomic<bool> isPlaying{false};
    std::atomic<bool> isPaused{false};
    std::vector<std::thread> graphicModuleThreads;
    std::unordered_map<GraphicModule*, std::condition_variable> cvs;
    std::unordered_map<GraphicModule*, std::mutex> cv_ms;

    void switchTemplate(int direction);
    void checkIfLogDirectoryExists();
    bool isValidLogDirectory(const std::string& directoryPath);
    void openLogDirectoryDialog();

    void processLogDirectoryDialog();
    void play();
    void pause();


//    void stop();
//
//    void handlePlayback();
//
//                          // Stop playback
//    void nextFrame();                  // Move to the next frame
//    void previousFrame();              // Move to the previous frame
//
//       // Manages templates and elements
//    //std::vector<std::pair<float, json>> replayData;  // Replay data: <timestamp, data>
//    size_t currentFrame = 0;           // Current playback frame
//    float playbackSpeed = 1.0f;        // Playback speed
//    std::chrono::time_point<std::chrono::steady_clock> lastUpdate; // Time of the last update
//          // Path to the log directory
//    std::vector<std::string> modules;  // List of modules to process


};
