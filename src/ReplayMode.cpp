#include "ReplayMode.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <stdexcept>

namespace fs = std::filesystem;

ReplayMode::ReplayMode(YAML::Node configFile) : GUI(configFile), templateManager(false) {
    if (configFile["logDirectory"]) {
        try {
            logDirectory = fs::canonical(configFile["logDirectory"].as<std::string>()).string();
            std::cout << "Log directory: " << logDirectory << std::endl;
        } catch (const fs::filesystem_error& e) {
            throw std::runtime_error("Invalid log directory: " + std::string(e.what()));
        }
    } else {
        throw std::runtime_error("Log directory not specified in configuration.");
    }

    if (configFile["modules"]) {
        for (const auto& module : configFile["modules"]) {
            modules.push_back(module.as<std::string>());
        }
        std::cout << "Modules loaded: ";
        for (const auto& mod : modules) {
            std::cout << mod << " ";
        }
        std::cout << std::endl;
    } else {
        throw std::runtime_error("Modules not specified in configuration.");
    }
}

int ReplayMode::run() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return 1;
    }

    loadLogData(logDirectory);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        drawMenuBar();
        handlePlayback();

        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(io.DisplaySize);

        ImGui::Begin("Replay Mode", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
        drawElements();
        ImGui::End();

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    cleanupImGui();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

void ReplayMode::loadLogData(const std::string& directoryPath) {
    replayData.clear();

    if (!fs::exists(directoryPath) || !fs::is_directory(directoryPath)) {
        throw std::runtime_error("Log directory does not exist or is not a directory: " + directoryPath);
    }

    for (const auto& timestampDir : fs::directory_iterator(directoryPath)) {
        if (timestampDir.is_directory()) {
            std::cout << "Processing timestamp directory: " << timestampDir.path().filename().string() << std::endl;

            for (const auto& moduleDir : fs::directory_iterator(timestampDir.path())) {
                if (moduleDir.is_directory()) {
                    std::string moduleName = moduleDir.path().filename().string();

                    if (std::find(modules.begin(), modules.end(), moduleName) != modules.end()) {
                        std::cout << "Processing module: " << moduleName << std::endl;

                        for (const auto& logFile : fs::directory_iterator(moduleDir.path())) {
                            if (logFile.is_regular_file() && logFile.path().extension() == ".json") {
                                processLogFile(logFile.path(), moduleName);
                            }
                        }
                    }
                }
            }
        }
    }

    std::sort(replayData.begin(), replayData.end(),
              [](const std::pair<float, json>& a, const std::pair<float, json>& b) {
                  return a.first < b.first;
              });

    std::cout << "Total frames loaded: " << replayData.size() << std::endl;
}

void ReplayMode::processLogFile(const fs::path& filePath, const std::string& moduleName) {
    std::ifstream inFile(filePath);
    if (!inFile.is_open()) {
        std::cerr << "Error: Could not open file " << filePath << std::endl;
        return;
    }

    json logJson;
    inFile >> logJson;

    if (logJson.contains("frames")) {
        for (const auto& frame : logJson["frames"]) {
            float timestamp = frame["timestamp"].get<float>();
            json data = frame["data"];
            data["module"] = moduleName;
            replayData.emplace_back(timestamp, data);
        }
    }
}

void ReplayMode::drawMenuBar() {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("Playback")) {
            if (ImGui::MenuItem("Play", nullptr, isPlaying)) play();
            if (ImGui::MenuItem("Pause", nullptr, !isPlaying)) pause();
            if (ImGui::MenuItem("Stop")) stop();
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

void ReplayMode::drawElements() {
    if (currentFrame < replayData.size()) {
        const auto& frameData = replayData[currentFrame].second;

        if (frameData.contains("module")) {
            std::string moduleName = frameData["module"];
            std::cout << "Drawing data for module: " << moduleName << std::endl;

            if (moduleName == "CounterModule" && frameData.contains("counter")) {
                int counter = frameData["counter"];
                ImGui::Text("Counter Value: %d", counter);
            }

            if (moduleName == "MapModule" && frameData.contains("rows") &&
                frameData.contains("cols") && frameData.contains("map")) {
                int rows = frameData["rows"];
                int cols = frameData["cols"];
                ImGui::Text("Map: %dx%d", rows, cols);
            }

            if (moduleName == "UltrasonicModule" && frameData.contains("angle") &&
                frameData.contains("distance")) {
                float angle = frameData["angle"];
                float distance = frameData["distance"];
                ImGui::Text("Ultrasonic Sensor - Angle: %.2f, Distance: %.2f", angle, distance);
            }
        } else {
            std::cout << "Frame data does not contain 'module' field." << std::endl;
        }
    } else {
        std::cout << "No frames available to draw." << std::endl;
    }
}

void ReplayMode::handlePlayback() {
    if (!isPlaying || replayData.empty()) return;

    auto now = std::chrono::steady_clock::now();
    if (std::chrono::duration<float>(now - lastUpdate).count() >= 1.0f / playbackSpeed) {
        lastUpdate = now;
        if (currentFrame < replayData.size() - 1) {
            currentFrame++;
        } else {
            stop();
        }
    }
}

void ReplayMode::play() {
    isPlaying = true;
    lastUpdate = std::chrono::steady_clock::now();
}

void ReplayMode::pause() {
    isPlaying = false;
}

void ReplayMode::stop() {
    isPlaying = false;
    currentFrame = 0;
}

void ReplayMode::setupShortcuts() {
    shortcutsManager.registerShortcut("Space", [this]() {
        if (isPlaying) pause();
        else play();
    });
    shortcutsManager.registerShortcut("Left", [this]() { previousFrame(); });
    shortcutsManager.registerShortcut("Right", [this]() { nextFrame(); });
}

void ReplayMode::nextFrame() {
    if (currentFrame < replayData.size() - 1) currentFrame++;
}

void ReplayMode::previousFrame() {
    if (currentFrame > 0) currentFrame--;
}
