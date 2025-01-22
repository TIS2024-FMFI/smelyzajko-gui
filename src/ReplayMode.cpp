#include "ReplayMode.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <stdexcept>
#include <regex>
#include "ImGuiFileDialog.h"





int ReplayMode::run() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return 1;
    }

    checkIfLogDirectoryExists();

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        shortcutsManager.processShortcuts();
        toastManager.renderNotifications();
//processLogDirectoryDialog
//        drawMenuBar();
        processLogDirectoryDialog();
        ImGui::SetNextWindowPos(ImVec2(0, 0)); // Top-left corner of the screen
        ImGui::SetNextWindowSize(io.DisplaySize); // Fullscreen size

        ImGui::Begin("Main Window", nullptr,
                     ImGuiWindowFlags_NoTitleBar |    // Remove title bar
                     ImGuiWindowFlags_NoCollapse |   // Prevent collapsing
                     ImGuiWindowFlags_NoResize |     // Disable resizing
                     ImGuiWindowFlags_NoMove |       // Prevent moving the window
                     ImGuiWindowFlags_NoBringToFrontOnFocus | // Prevent window focus changes
                     ImGuiWindowFlags_NoScrollbar    // Disable scrollbar (optional)
        );


        ImGui::End();
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);

    }

    for (Element* element : templateManager.getActiveTemplateElements()) {
        delete element;
    }
    templateManager.clearActiveTemplateElements();

    for (GraphicModule* module : templateManager.getActiveTemplateModules()) {
        delete module;
    }
    cleanupImGui();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
void ReplayMode::startGraphicModuleThreads() {
    for (GraphicModule* module : moduleManager.getGraphicModules()) {
        graphicModuleThreads.emplace_back(&ReplayMode::runGraphicModule, this, module);
    }
}
void ReplayMode::runGraphicModule(GraphicModule* module) {
    // Implement the logic to run in the thread for each GraphicModule
    int frequency = module->getGraphicsFrequency();
    std::chrono::milliseconds interval;


    if (frequency > 0) {
        interval = std::chrono::milliseconds(60000 / frequency);
    } else {
        interval = std::chrono::milliseconds(0);
    }

    while (isPlaying) {
        // Call a method on the module
        module->logForward();
        // Add a sleep or wait mechanism to control the thread execution
        if (interval.count() > 0) {
            std::this_thread::sleep_for(interval);
        } else {
            std::this_thread::yield(); // Yield to other threads if frequency is 0
        }
    }
}

//void ReplayMode::processLogFile(const fs::path& filePath, const std::string& moduleName) {
//    std::ifstream inFile(filePath);
//    if (!inFile.is_open()) {
//        std::cerr << "Error: Could not open file " << filePath << std::endl;
//        return;
//    }
//
//    json logJson;
//    inFile >> logJson;
//
//    if (logJson.contains("frames")) {
//        for (const auto& frame : logJson["frames"]) {
//            float timestamp = frame["timestamp"].get<float>();
//            json data = frame["data"];
//            data["module"] = moduleName;
//            replayData.emplace_back(timestamp, data);
//        }
//    }
//}
//
//void ReplayMode::drawMenuBar() {
//    if (ImGui::BeginMainMenuBar()) {
//        if (ImGui::BeginMenu("Playback")) {
//            if (ImGui::MenuItem("Play", nullptr, isPlaying)) play();
//            if (ImGui::MenuItem("Pause", nullptr, !isPlaying)) pause();
//            if (ImGui::MenuItem("Stop")) stop();
//            ImGui::EndMenu();
//        }
//        ImGui::EndMainMenuBar();
//    }
//}
//
//
//void ReplayMode::handlePlayback() {
//    if (!isPlaying || replayData.empty()) return;
//
//    auto now = std::chrono::steady_clock::now();
//    if (std::chrono::duration<float>(now - lastUpdate).count() >= 1.0f / playbackSpeed) {
//        lastUpdate = now;
//        if (currentFrame < replayData.size() - 1) {
//            currentFrame++;
//        } else {
//            stop();
//        }
//    }
//}
//
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
    shortcutsManager.setWindow(window);
//    shortcutsManager.registerShortcut("Space", [this]() {
//        if (isPlaying) pause();
//        else play();
//    });
//    shortcutsManager.registerShortcut("Left", [this]() { previousFrame(); });
//    shortcutsManager.registerShortcut("Right", [this]() { nextFrame(); });

    shortcutsManager.registerShortcut("Ctrl+Left", [this]() {
        switchTemplate(-1);
    });
    shortcutsManager.registerShortcut("Ctrl+Right", [this]() {
        switchTemplate(1);
    });

    // Apple (use CMD)
    shortcutsManager.registerShortcut("Cmd+Left", [this]() {
        switchTemplate(-1);
    });
    shortcutsManager.registerShortcut("Cmd+Right", [this]() {
        switchTemplate(1);
    });

    for (Module* module : moduleManager.getModules()) {
        module->registerShortcuts(shortcutsManager, toastManager);
    }
}
void ReplayMode::switchTemplate(int direction) {
    if (templateManager.getAllTemplates().empty()) {
        std::cerr << "No templates to switch between." << std::endl;
        return;
    }

    // Update the index (wrap around if necessary)
    int templateCount = templateManager.getAllTemplates().size();
    currentTemplateIndex = (currentTemplateIndex + direction + templateCount) % templateCount;

    // Activate the new template
    Template activeTemplate = templateManager.getAllTemplates()[currentTemplateIndex];
    templateManager.setActiveTemplate(activeTemplate);
    std::string activeTemplateName = templateManager.getActiveTemplateName();
    std::string windowTitle = std::string("GUI") + " - " + activeTemplateName;
    glfwSetWindowTitle(window, windowTitle.c_str());
}

void ReplayMode::loadLogData() {

    if (!configFile["logDirectory"] || configFile["logDirectory"].as<std::string>().empty()) {
        throw std::runtime_error("Log directory not specified in the configuration file.");
    }
    std::cout<<"loadLogData"<<std::endl;
    std::filesystem::path logDirPath = configFile["logDirectory"].as<std::string>();

    if (!std::filesystem::exists(logDirPath) || !std::filesystem::is_directory(logDirPath)) {
        throw std::runtime_error("Log directory does not exist or is not a directory: " + logDirPath.string());
    }

    for (const auto& moduleDir : std::filesystem::directory_iterator(logDirPath)) {
        if (moduleDir.is_directory()) {
            std::string moduleName = moduleDir.path().filename().string();
            for (const auto& subDir : std::filesystem::directory_iterator(moduleDir.path())) {
                std::string subDirName = subDir.path().filename().string();
                if (subDirName.size() > 5 && subDirName.substr(subDirName.size() - 5) == ".json") {
                    subDirName = subDirName.substr(0, subDirName.size() - 5);  // Remove the ".json" part
                }
                int graphicElementID = moduleManager.registerGraphicModule(subDirName, moduleName,0);
                moduleManager.setLogDirectory(0, graphicElementID, moduleDir.path().string());
            }
        }
    }
}

void ReplayMode::checkIfLogDirectoryExists() {
    std::cout<<"checkIfLogDirectoryExists"<<std::endl;
    if (configFile["logDirectory"]) {
        logDirectory = configFile["logDirectory"].as<std::string>();
        std::filesystem::path logDirPath(logDirectory);
        std::string dirName = logDirPath.filename().string();
        if (!isValidLogDirectory(dirName)) {
            std::cerr << "[ERROR] Log directory does not match the required format. Please select a valid directory." << std::endl;
            openLogDirectoryDialog();
        }else{
            loadLogData();
        }
    } else {
        std::cerr << "[ERROR] No log directory specified in the configuration file." << std::endl;
        openLogDirectoryDialog();
    }
}

bool ReplayMode::isValidLogDirectory(const std::string& dirName) {
    std::regex pattern(R"(\d{4}-\d{2}-\d{2}_\d{2}:\d{2}:\d{2})");
    return std::regex_match(dirName, pattern);
}

void ReplayMode::openLogDirectoryDialog() {

    if (ImGuiFileDialog::Instance()->IsOpened("ChooseLogDirDlgKey")) {
        return;
    }
    IGFD::FileDialogConfig config;
    config.path = "../logs";
    ImGuiFileDialog::Instance()->OpenDialog("ChooseLogDirDlgKey", "Choose Directory for logging", nullptr, config);
}

void ReplayMode::processLogDirectoryDialog() {
    if (ImGuiFileDialog::Instance()->Display("ChooseLogDirDlgKey", ImGuiFileDialogFlags_Modal)) {
        if (ImGuiFileDialog::Instance()->IsOk()) {
            std::filesystem::path logDirPath = ImGuiFileDialog::Instance()->GetCurrentPath();
            std::string dirName = logDirPath.filename().string();

            if (isValidLogDirectory(dirName)) {
                try {
                    if (!std::filesystem::exists(logDirPath)) {
                        std::filesystem::create_directories(logDirPath);
                    }

                    configFile["logDirectory"] = logDirPath.string();
                    loadLogData();
                    ImGuiFileDialog::Instance()->Close();

                } catch (const std::exception& e) {
                    std::cerr << "Error creating log directory: " << e.what() << std::endl;
                }
            } else {
                std::cerr << "[ERROR] Selected directory does not match the required format. Please select a valid directory." << std::endl;
                openLogDirectoryDialog(); // Reopen the dialog to select again
                toastManager.addNotification("Error", "Selected directory does not match the required format. Please select a valid directory.");
            }
        } else {
            // Close the directory dialog if canceled
            ImGuiFileDialog::Instance()->Close();
        }
    }
}




//
//void ReplayMode::nextFrame() {
//    if (currentFrame < replayData.size() - 1) currentFrame++;
//}
//
//void ReplayMode::previousFrame() {
//    if (currentFrame > 0) currentFrame--;
//}
