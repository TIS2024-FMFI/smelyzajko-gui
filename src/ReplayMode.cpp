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
        if (!templateManager.getActiveTemplateModules().empty()) {
            moduleManager.setActiveModuleAndDraw(templateManager.getActiveTemplateModules(),io);
        }
        shortcutsManager.processShortcuts();
        toastManager.renderNotifications();

        drawMenuBar();
        processLogDirectoryDialog();

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
    for (Module* module : moduleManager.getModules()) {
        delete module;
    }
    for (GraphicModule* module : moduleManager.getGraphicModules()) {
        delete module;
    }
    stopGraphicModuleThreads();
    cleanupImGui();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}



void ReplayMode::startGraphicModuleThreads() {
    isPlaying = true;
    auto modules = moduleManager.getGraphicModules();

    for (GraphicModule* module : modules) {
        module->logFromJson();
        graphicModuleThreads.emplace_back(&ReplayMode::runGraphicModule, this, module);
    }
}



void ReplayMode::runGraphicModule(GraphicModule* module) {
    int frequency = (module->getGraphicElementName() == "TextArea") ? module->getTextFrequency() : module->getGraphicsFrequency();

    std::chrono::milliseconds interval;

    if (frequency > 0) {
        interval = std::chrono::milliseconds(60000 / frequency);
    } else {

        std::cerr << "Invalid frequency for module " << module->getModuleName() <<" and for graphic element: "<<module->getGraphicElementName()<< std::endl;
        return;
    }

    while (isPlaying) {
        std::unique_lock<std::mutex> lock(cv_ms[module]);
        cvs[module].wait(lock, [this] { return !isPaused.load(); });

        module->logForward();

        if (interval.count() > 0) {
            std::this_thread::sleep_for(interval);
        } else {
            std::this_thread::yield();
        }
    }
}

void ReplayMode::stopGraphicModuleThreads() {
    isPlaying = false;
    for (std::thread& thread : graphicModuleThreads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
    graphicModuleThreads.clear();
}

void ReplayMode::drawMenuBar() {
    if (ImGui::BeginMainMenuBar()) {

        // Templates Dropdown
        if (ImGui::BeginMenu("Templates")) {
            for (const Template& aTemplate : templateManager.getAllTemplates()) {
                if (ImGui::MenuItem(aTemplate.getName().c_str())) {
                    templateManager.setActiveTemplate(aTemplate);
                    std::string activeTemplateName = templateManager.getActiveTemplateName();
                    std::string windowTitle = std::string("GUI") + " - " + activeTemplateName;
                    glfwSetWindowTitle(window, windowTitle.c_str());
                }
            }
            ImGui::EndMenu();
        }

        // Control Options
        if (ImGui::MenuItem("Play", nullptr, isPlaying)) play();
        if (ImGui::MenuItem("Pause", nullptr, !isPlaying)) pause();
        if (ImGui::MenuItem("Back")) back();
        if (ImGui::MenuItem("Stop")) stopGraphicModuleThreads();

        // Config Dropdown
        if (ImGui::BeginMenu("Config")) {
            ImGui::Text("Set Fixed Time Step"); // Label for the dropdown

            // Input field for manual time step adjustment
            ImGui::InputInt("Time Step (ms)", &fixedTimeStep);

            // Slider for easier adjustment within a range
            ImGui::SliderInt("Adjust Time Step", &fixedTimeStep, 50, 2000, "%d ms");
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}




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
    isPaused = false;
    for (auto& [module, cv] : cvs){
        cv.notify_all();
    }
}

void ReplayMode::pause() {
    isPaused = true;
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
    for (GraphicModule* module : moduleManager.getGraphicModules()) {
        accumulatedTime[module] = 0;
    }
    startGraphicModuleThreads();
}

void ReplayMode::checkIfLogDirectoryExists() {
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
    std::regex pattern(R"(\d{4}-\d{2}-\d{2}_\d{2}-\d{2}-\d{2})");
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

void ReplayMode::back() {
    isPaused = true; // Pause playback to allow precise navigation

    for (GraphicModule* module : moduleManager.getGraphicModules()) {
        int frequency = (module->getGraphicElementName() == "TextArea") ? module->getTextFrequency() : module->getGraphicsFrequency();
        int interval = 60000 / frequency;
        // Update accumulated time for the module
        accumulatedTime[module] += fixedTimeStep;

        if (interval > 0) {
        while (accumulatedTime[module] >= interval) {
            module->logBackwards();
            accumulatedTime[module] -= interval; // Reset accumulated time
        }
        }
    }
}






