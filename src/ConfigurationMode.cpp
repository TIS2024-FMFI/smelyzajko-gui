#include <iostream>
#include "ConfigurationMode.h"
#include "TestModules/CounterModule.h"
#include "TestModules/MapModule.h"

#include <iostream> // For std::cerr (debugging)
#ifdef _WIN32
#include <windows.h> // For Beep on Windows
#else
#include <unistd.h> // For usleep on Unix-based systems
#endif


#if defined(_WIN32) || defined(_WIN64)
#include <Windows.h>
void playBeep() {
    MessageBeep(MB_ICONEXCLAMATION); // Standard system beep
}
#elif defined(__linux__) || defined(__APPLE__)
#include <iostream>
void playBeep() {
    std::cout << "\a"; // Linux/macOS beep
}
#endif


void ConfigurationMode::initializeModules() {
    ModuleManager& moduleManager = ModuleManager::getInstance();

//    // Registering CounterModule and MapModule
//    moduleManager.registerModule("Map Module", new MapModule(&moduleManager));
//    moduleManager.registerModule("Counter Module", new CounterModule(&moduleManager));

    new MapModule(&moduleManager);
    new CounterModule(&moduleManager);

}


inline bool isOverlapping(const ImRect& a, const ImRect& b) {
    return !(a.Max.x <= b.Min.x ||  // No overlap on the left
             a.Min.x >= b.Max.x ||  // No overlap on the right
             a.Max.y <= b.Min.y ||  // No overlap above
             a.Min.y >= b.Max.y);   // No overlap below
}

ImVec2 findFreePosition(const std::vector<Element*>& elements, const ImVec2& elementSize, const ImVec2& start = ImVec2(10.0f, 10.0f), float step = 10.0f, float padding = 15.0f, float menuHeight = 20.0f) {
    ImVec2 position = start;
    bool isPositionFree;

    // Get display dimensions
    const ImVec2 displaySize = ImGui::GetIO().DisplaySize;

    do {
        isPositionFree = true;

        // Calculate the bounding box for the new element
        ImRect newBoundingBox(
                ImVec2(position.x - padding, position.y - padding),
                ImVec2(position.x + elementSize.x + padding, position.y + elementSize.y + padding)
        );

        // Check if the position is fully visible on the screen
        if (newBoundingBox.Min.x < 0 || newBoundingBox.Min.y < menuHeight ||
            newBoundingBox.Max.x > displaySize.x || newBoundingBox.Max.y > displaySize.y) {
            isPositionFree = false;
        }

        // Check overlap with existing elements
        if (isPositionFree) {
            for (const auto& element : elements) {
                if (isOverlapping(newBoundingBox, element->getBoundingBox())) {
                    isPositionFree = false;
                    break;
                }
            }
        }

        // Move to the next position if the current position is not free
        if (!isPositionFree) {
            position.x += step; // Move horizontally
            if (position.x + elementSize.x + padding > displaySize.x) {
                position.x = start.x; // Reset X to the start
                position.y += step;  // Move vertically
            }
        }

    } while (!isPositionFree && position.y + elementSize.y + padding <= displaySize.y);

    // Return invalid position if no valid position is found
    if (!isPositionFree) {
        return ImVec2(-1.0f, -1.0f);
    }

    return position;
}

ImVec2 findNearestFreeGridCorner(const std::vector<Element*>& elements, const ImVec2& elementSize, float gridSize, const ImVec2& padding, float menuBarHeight) {
    ImVec2 screenSize = ImGui::GetIO().DisplaySize; // Get screen dimensions

    // Iterate over grid positions row by row
    for (float y = menuBarHeight; y + elementSize.y <= screenSize.y; y += gridSize) { // Ensure it fits vertically
        for (float x = 0.0f; x + elementSize.x <= screenSize.x; x += gridSize) {     // Ensure it fits horizontally
            ImVec2 position = ImVec2(x, y);

            // Create a bounding box for the new rectangle
            ImRect newRect(position, ImVec2(position.x + elementSize.x, position.y + elementSize.y));
            bool overlaps = false;

            // Check for overlaps with existing elements
            for (const auto& element : elements) {
                ImRect existingRect = element->getBoundingBox();

                // Add padding around existing elements for spacing
                existingRect.Min.x -= padding.x;
                existingRect.Min.y -= padding.y;
                existingRect.Max.x += padding.x;
                existingRect.Max.y += padding.y;

                if (newRect.Overlaps(existingRect)) {
                    overlaps = true;
                    break;
                }
            }

            // If no overlap is found and the rectangle fits on the screen, return this position
            if (!overlaps) {
                return position;
            }
        }
    }

    // Fallback if no position is found
    return ImVec2(-1.0f, menuBarHeight);
}

void renderSettingsPopupForModule(const std::string& moduleName, const std::string& jsonFilePath) {
    static int frequency = 0;
    static bool logEnabled = false;
    static bool settingsVisible = false;

    static std::string currentModuleName;
    static nlohmann::json config;

    // Load JSON if not already loaded
    if (currentModuleName != moduleName) {
        currentModuleName = moduleName;

        // Open JSON file
        std::ifstream file(jsonFilePath);
        if (!file.is_open()) {
            std::cerr << "Failed to open JSON file: " << jsonFilePath << std::endl;
            return;
        }

        file >> config;

        // Load the specific module's settings
        for (const auto& module : config["graphicModules"]) {
            if (module["name"] == moduleName) {
                frequency = static_cast<int>(module["graphicsFrequency"]);
                logEnabled = module["graphicsLogEnabled"];
                settingsVisible = true;
                break;
            }
        }
    }

    // Render the settings window
    if (settingsVisible) {
        std::string windowTitle = "Settings: " + moduleName;

        ImGui::Begin(windowTitle.c_str(), &settingsVisible, ImGuiWindowFlags_AlwaysAutoResize);

        ImGui::Text("Module: %s", moduleName.c_str());
        ImGui::SliderInt("Graphics Frequency", &frequency, 0, 100);
        ImGui::Checkbox("Enable Logging", &logEnabled);

        ImGui::NewLine();

        // Apply button to save changes
        if (ImGui::Button("Apply")) {
            for (auto& module : config["graphicModules"]) {
                if (module["name"] == moduleName) {
                    module["graphicsFrequency"] = frequency;
                    module["graphicsLogEnabled"] = logEnabled;
                    break;
                }
            }

            // Save changes to the JSON file
            std::ofstream outFile(jsonFilePath);
            if (!outFile.is_open()) {
                std::cerr << "Failed to write to JSON file: " << jsonFilePath << std::endl;
            } else {
                outFile << config.dump(4); // Save formatted JSON
                std::cout << "Updated settings for " << moduleName << " in JSON file." << std::endl;
            }
            settingsVisible = false; // Close the settings window
        }

        ImGui::SameLine();

        // Cancel button to discard changes
        if (ImGui::Button("Cancel")) {
            settingsVisible = false; // Close the settings window without applying changes
        }

        ImGui::End();
    }
}

void ConfigurationMode::setupShortcuts() {

    shortcutsManager.registerShortcut("Ctrl+Q", [this]() {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    });

}

void ConfigurationMode::processShortcuts() {
    shortcutsManager.processShortcuts();
}

void ConfigurationMode::initializeWindow(GLFWwindow* window) {
    this->window = window;
    shortcutsManager.setWindow(window);
}


int ConfigurationMode::run() {

    initializeWindow(window);
    shortcutsManager.setWindow(window);
    setupShortcuts();
    initializeModules();

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        processShortcuts();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        setupMenuBar();

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


            if (isSnapping){
                drawElementsWithSnappingOn();
            }else{
                drawElements();
            }

        ImGui::End();

        if (showGrid) drawGrid();

        // render all toast notifications
        toastManager.renderNotifications();

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    for (Element* element : templateManager.getActiveTemplateElements()) {
        delete element;
    }

    templateManager.clearActiveTemplateElements();
    cleanupImGui();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

void ConfigurationMode::drawElements() {
    // First, draw the rectangles
    auto activeElements = templateManager.getActiveTemplateElements();
    for (int i = 0; i < activeElements.size(); i++) {
        Element *element = activeElements[i];
        ImGui::PushID(i);

        // Draw the element
        element->draw(io);

        ImGui::PopID();
    }

    // Check for clicks and move the element to top
    Element *clickedElement = nullptr;
    for (int i = activeElements.size() - 1; i >= 0; i--) {
        Element *element = activeElements[i];
        ImRect bbox = element->getBoundingBox();

        if (bbox.Contains(ImGui::GetMousePos())) {
            if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
                clickedElement = element;
                break;
            }
        }
    }
    // If an element is clicked, bring it to the top
    if (clickedElement) {
        bringElementToTop(clickedElement);
    }


    bool clickHandled = false;
    for (int i = activeElements.size() - 1; i >= 0; i--) {
        Element *element = activeElements[i];
        ImGui::PushID(i);

        element->handleClicks(io);

        ImRect bbox = element->getBoundingBox();
        if (bbox.Contains(ImGui::GetMousePos())) {
            if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
                element->detectRightClickDelete();
                clickHandled = true;
            }
        }

        if (element->getPendingDelete()) {
            ImGui::SetNextWindowPos(element->getDeletePopupPosition(), ImGuiCond_Always);
            ImGui::OpenPopup("Delete Confirmation");

            if (ImGui::BeginPopupModal("Delete Confirmation", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
                ImGui::Text("Delete this element?");
                if (ImGui::Button("Yes")) {
                    templateManager.removeElementFromActiveTemplate(i);
                    ImGui::CloseCurrentPopup();
                    ImGui::EndPopup();
                    ImGui::PopID();
                    break;
                }
                ImGui::SameLine();
                if (ImGui::Button("No")) {
                    element->setPendingDelete(false);
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndPopup();
            }
        }

        ImGui::PopID();

        if (clickHandled) {
            break;
        }
    }
}

void ConfigurationMode::drawElementsWithSnappingOn() {
    auto activeElements = templateManager.getActiveTemplateElements();
    static Element* draggedElement = nullptr; // Track the currently dragged element

    for (int i = 0; i < activeElements.size(); i++) {
        Element* element = activeElements[i];
        ImGui::PushID(i);

        // Get the element's current position
        ImVec2 elementPos = element->getPosition();

        // Handle mouse down to start dragging
        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
            ImRect bbox = element->getBoundingBox();
            if (bbox.Contains(ImGui::GetMousePos())) {
                draggedElement = element; // Set this element as the one being dragged
            }
        }

        // Handle dragging for the dragged element only
        if (draggedElement == element && ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
            ImVec2 dragDelta = ImGui::GetMouseDragDelta();
            elementPos.x += dragDelta.x;
            elementPos.y += dragDelta.y;

            ImGui::ResetMouseDragDelta();     // Reset drag delta
        }

        // Snap only the dragged element when releasing the mouse button
        if (draggedElement == element && ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
            elementPos.x = round(elementPos.x / gridSize) * gridSize;
            elementPos.y = round((elementPos.y - menuBarHeight) / gridSize) * gridSize + menuBarHeight;

            element->setPosition(elementPos); // Apply snapped position
            draggedElement = nullptr;         // Clear dragged element
        }

        // Draw the element
        element->draw(io);
        ImGui::PopID();
    }

    // Check for clicks and move the element to top
    Element *clickedElement = nullptr;
    for (int i = activeElements.size() - 1; i >= 0; i--) {
        Element *element = activeElements[i];
        ImRect bbox = element->getBoundingBox();

        if (bbox.Contains(ImGui::GetMousePos())) {
            if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
                clickedElement = element;
                break;
            }
        }
    }
    // If an element is clicked, bring it to the top
    if (clickedElement) {
        bringElementToTop(clickedElement);
    }

    bool clickHandled = false;
    for (int i = activeElements.size() - 1; i >= 0; i--) {
        Element *element = activeElements[i];
        ImGui::PushID(i);

        element->handleClicks(io);

        ImRect bbox = element->getBoundingBox();
        if (bbox.Contains(ImGui::GetMousePos())) {
            if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
                element->detectRightClickDelete();
                clickHandled = true;
            }
        }

        if (element->getPendingDelete()) {
            ImGui::SetNextWindowPos(element->getDeletePopupPosition(), ImGuiCond_Always);
            ImGui::OpenPopup("Delete Confirmation");

            if (ImGui::BeginPopupModal("Delete Confirmation", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
                ImGui::Text("Delete this element?");
                if (ImGui::Button("Yes")) {
                    templateManager.removeElementFromActiveTemplate(i);
                    ImGui::CloseCurrentPopup();
                    ImGui::EndPopup();
                    ImGui::PopID();
                    break;
                }
                ImGui::SameLine();
                if (ImGui::Button("No")) {
                    element->setPendingDelete(false);
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndPopup();
            }
        }

        ImGui::PopID();

        if (clickHandled) {
            break;
        }
    }
}




void ConfigurationMode::setupMenuBar() {
    if (ImGui::BeginMainMenuBar()) {
        menuBarHeight = ImGui::GetWindowHeight();
        if (ImGui::BeginMenu("Templates")) {
            if (ImGui::MenuItem("New template")) {
                templateManager.setActiveTemplate(Template());
                std::string windowTitle = std::string("GUI");
                glfwSetWindowTitle(window, windowTitle.c_str());
            }
            if (ImGui::BeginMenu("Templates")) {
                for (const Template &aTemplate: templateManager.getAllTemplates()) {
                    if (ImGui::MenuItem(aTemplate.getName().c_str())) {
                        templateManager.setActiveTemplate(aTemplate);
                        std::string activeTemplateName = templateManager.getActiveTemplateName();
                        std::string windowTitle = std::string("GUI") + " - " + activeTemplateName;
                        glfwSetWindowTitle(window, windowTitle.c_str());
                    }
                }
                ImGui::EndMenu();
            }
            if (ImGui::Button("Save current template")) {
                IGFD::FileDialogConfig config;
                config.path = "../templates"; // default path for the file dialog
                if (!templateManager.getActiveTemplateName().empty()) {
                    config.fileName = templateManager.getActiveTemplateName() + ".json";
                }
                ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".json", config);
            }
            // display the dialog and handle the file selection
            if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey")) {
                if (ImGuiFileDialog::Instance()->IsOk()) {
                    std::filesystem::path filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
                    try {
                        bool isNew = false;
                        std::string templateName = templateManager.getActiveTemplate().getName();
                        std::string fileName = filePathName.filename().string();
                        if (fileName.size() > 5 && fileName.substr(fileName.size() - 5) == ".json") {
                            fileName = fileName.substr(0, fileName.size() - 5);  // Remove the ".json" part
                        }

                        if (templateName.empty() || templateName != fileName ) {
                            isNew = true;
                        }

                        if (isNew) {
                            templateManager.getActiveTemplate().saveTemplate(filePathName, fileName);
                            Template newTemplate = Template("../templates/" + filePathName.filename().string());
                            templateManager.allTemplates.push_back(newTemplate);
                            templateManager.setActiveTemplate(newTemplate);
                            std::string windowTitle = std::string("GUI") + " - " + newTemplate.getName();
                            glfwSetWindowTitle(window, windowTitle.c_str());
                        } else {
                            templateManager.getActiveTemplate().saveTemplate(filePathName);
                        }
                    } catch (const std::exception& e) {
                        std::cerr << "Error saving template: " << e.what() << std::endl;
                    }
                }

                ImGuiFileDialog::Instance()->Close();
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Options")) {

            ImGui::Checkbox("Enable Snapping", &isSnapping);
            ImGui::Checkbox("Show Grid", &showGrid);

            ImGui::InputFloat("Grid Size", &gridSize, 5.0f, 1.0f);

            // Minimum value to avoid dividing by 0
            if (gridSize < minGridValue) {
                gridSize = minGridValue;
            }else if (gridSize > maxGridValue){
                gridSize = maxGridValue;
            }

            ImGui::EndMenu();
        }

        static nlohmann::json config;
        static bool isConfigLoaded = false;

        // Load JSON once
        if (!isConfigLoaded) {
            std::ifstream file("../templates/template1.json");
            if (!file.is_open()) {
                std::cerr << "Failed to open JSON file: " << "../templates/template1.json" << std::endl;
                return;
            }
            file >> config;
            isConfigLoaded = true;
        }

        // Render the Configuration menu
        if (ImGui::BeginMenu("Configuration")) {
            static std::set<std::string> uniqueModules;

            // Iterate through modules to populate the menu
            for (const auto& module : config["graphicModules"]) {
                std::string moduleName = module["name"];
                if (uniqueModules.find(moduleName) == uniqueModules.end()) {
                    uniqueModules.insert(moduleName);

                    // Create a menu item for each unique module
                    if (ImGui::MenuItem(moduleName.c_str())) {
                        renderSettingsPopupForModule(moduleName, "../templates/template1.json");
                    }
                }
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Controls")) {
            auto elements = templateManager.getActiveTemplateElements();

            if (ImGui::MenuItem("Add Rectangle")) {
                ImVec2 elementSize(300.0f, 200.0f);
                ImVec2 padding(30.0f, 30.0f);
                float menuBarHeight = 25.0f;
                ImVec2 position;

                if (isSnapping) {
                    int widthInSquares = ceil(elementSize.x / gridSize);
                    int heightInSquares = ceil(elementSize.y / gridSize);
                    elementSize = ImVec2(widthInSquares * gridSize, heightInSquares * gridSize);

                    position = findNearestFreeGridCorner(elements, elementSize, gridSize, padding, menuBarHeight);
                } else {
                    position = findFreePosition(elements, elementSize, padding, 20.0f, 20.0f, menuBarHeight);
                }

                if (position.x == -1.0f && position.y == -1.0f) { // No free position found
                    playBeep();
                    position = ImVec2(0.0f, menuBarHeight); // Default to the top-left corner
                }

                addElementToActiveTemplate(new class Rectangle("Rectangle", position, elementSize));
            }


            if (ImGui::MenuItem("Add Checkbox")) {
                ImVec2 elementSize(30.0f, 30.0f);
                ImVec2 padding(10.0f, 10.0f);
                float menuBarHeight = 25.0f;
                ImVec2 position;

                if (isSnapping) {
                    elementSize = ImVec2(gridSize, gridSize);
                    position = findNearestFreeGridCorner(elements, elementSize, gridSize, padding, menuBarHeight);
                } else {
                    position = findFreePosition(elements, elementSize, padding, 20.0f, 20.0f, menuBarHeight);
                }

                if (position.x == -1.0f && position.y == -1.0f) { // No free position found
                    playBeep();
                    position = ImVec2(0.0f, menuBarHeight); // Default to the top-left corner
                }

                addElementToActiveTemplate(new Checkbox("Checkbox", position, false));
            }

            if (ImGui::MenuItem("Add Button")) {
                ImVec2 elementSize(100.0f, 25.0f);
                ImVec2 padding(20.0f, 20.0f);
                float menuBarHeight = 25.0f;
                ImVec2 position;

                if (isSnapping) {
                    elementSize = ImVec2(gridSize, gridSize);
                    position = findNearestFreeGridCorner(elements, elementSize, gridSize, padding, menuBarHeight);
                } else {
                    position = findFreePosition(elements, elementSize, padding, 20.0f, 20.0f, menuBarHeight);
                }

                if (position.x == -1.0f && position.y == -1.0f) { // No free position found
                    playBeep();
                    position = ImVec2(0.0f, menuBarHeight); // Default to the top-left corner
                }

                addElementToActiveTemplate(new Button("Button", position, elementSize));
            }


            if (ImGui::BeginMenu("Create Slider")) {
                createIntSliderSettings();
                createFloatSliderSettings();
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Create Label")) {
                createLabelSettings();
                ImGui::EndMenu();
            }

            ImGui::EndMenu();
        }


    }

    static char toastInputBuffer[256] = "";
    if (ImGui::BeginMenu("Test Notifications")) {
        ImGui::Text("Enter Notification:");
        ImGui::InputText("##Input", toastInputBuffer, IM_ARRAYSIZE(toastInputBuffer));

        if (ImGui::Button("Send Notification")) {
            if (strlen(toastInputBuffer) > 0) {
                toastManager.addNotification(toastInputBuffer);
                toastInputBuffer[0] = '\0'; // Clear the input buffer
            }
        }

        ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();
}










void ConfigurationMode::drawGrid() const {
    ImVec2 displaySize = io.DisplaySize;
    float menuBarHeight = ImGui::GetFrameHeight();
    ImDrawList *drawList = ImGui::GetBackgroundDrawList();

    ImU32 whiteColor = IM_COL32_WHITE;

    for (float x = 0; x < displaySize.x; x += gridSize) {
        drawList->AddLine(ImVec2(x, menuBarHeight), ImVec2(x, displaySize.y), whiteColor);
    }

    for (float y = menuBarHeight; y < displaySize.y; y += gridSize) {
        drawList->AddLine(ImVec2(0, y), ImVec2(displaySize.x, y), whiteColor);
    }
}

void ConfigurationMode::bringElementToTop(Element* element) {
    // Remove the clicked element and add it to the end of the list
    auto activeElements = templateManager.getActiveTemplateElements();
    auto it = std::find(activeElements.begin(), activeElements.end(), element);
    if (it != activeElements.end()) {
        activeElements.erase(it);
        activeElements.push_back(element);
    }

    for (int i = 0; i < activeElements.size(); ++i) {
        activeElements[i]->setZIndex(i);
    }
}

void ConfigurationMode::createLabelSettings() {
    if (ImGui::Button("Add Label")) {
        ImGui::OpenPopup("Add Label Popup");
    }

    if (ImGui::BeginPopup("Add Label Popup")) {
        static bool isMultiLine = false;
        static char text[256] = "Hello, World!";
        static ImVec2 position = ImVec2(100.0f, 100.0f);

        ImGui::Checkbox("Multi-line", &isMultiLine);
        ImGui::InputTextMultiline("Text", text, IM_ARRAYSIZE(text));

        // Calculate free position for the label
        auto elements = templateManager.getActiveTemplateElements();
        ImVec2 textSize = ImGui::CalcTextSize(text);
        ImVec2 labelSize = isMultiLine ? ImVec2(textSize.x, textSize.y) : ImVec2(textSize.x, 25.0f); // Adjust the height for single-line labels
        ImVec2 padding(10.0f, 10.0f); // Define padding to maintain space between elements


        if (isSnapping) {
            int widthInSquares = ceil(labelSize.x / gridSize);
            int heightInSquares = ceil(labelSize.y / gridSize);
            labelSize = ImVec2(widthInSquares * gridSize, heightInSquares * gridSize);

            position = findNearestFreeGridCorner(elements, labelSize, gridSize, padding, menuBarHeight);
        } else {
            position = findFreePosition(elements, labelSize, padding, 20.0f, 20.0f, menuBarHeight);
        }

        if (position.x == -1.0f && position.y == -1.0f) { // No free position found
            playBeep();
            position = ImVec2(0.0f, menuBarHeight); // Default to the top-left corner
        }

        labelSize = isMultiLine ? ImVec2(textSize.x, textSize.y) : ImVec2(textSize.x, 25.0f); // Adjust the height for single-line labels


        if (ImGui::Button("Add")) {
            if (isMultiLine) {
                addElementToActiveTemplate(new MultiLineLabel(
                        text,
                        position,
                        labelSize
                ));
            } else {
                // Sanitize text (remove newlines, replace with space)
                for (size_t i = 0; i < strlen(text); ++i) {
                    if (text[i] == '\n') {
                        text[i] = ' '; // Replace newline with space
                    }
                }
                addElementToActiveTemplate(new SingleLineLabel(
                        text,
                        position,
                        labelSize
                ));
            }
            ImGui::CloseCurrentPopup();
        }

        ImGui::SameLine();
        if (ImGui::Button("Cancel")) {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}


void ConfigurationMode::createIntSliderSettings() {
    if (ImGui::Button("Add Int Slider")) {
        ImGui::OpenPopup("Add Int Slider Popup");
    }
    if (ImGui::BeginPopup("Add Int Slider Popup")) {
        static char label[128] = "Slider (int)";
        static ImVec2 position = ImVec2(100.0f, 100.0f);
        static int minValue = 0;
        static int maxValue = 10;
        static int initialValue = 5;

        ImGui::InputText("Label", label, IM_ARRAYSIZE(label));
        ImGui::InputInt("Min Value", &minValue);
        ImGui::InputInt("Max Value", &maxValue);
        ImGui::InputInt("Initial Value", &initialValue);

        if (initialValue < minValue) initialValue = minValue;
        if (initialValue > maxValue) initialValue = maxValue;

        // Calculate free position for the int slider
        auto elements = templateManager.getActiveTemplateElements();
        ImVec2 sliderSize(200.0f, 20.0f); // Fixed size for the slider
        ImVec2 padding(10.0f, 10.0f); // Define padding to maintain space between elements

        if (isSnapping) {
            int widthInSquares = ceil(sliderSize.x / gridSize);
            int heightInSquares = ceil(sliderSize.y / gridSize);
            sliderSize = ImVec2(widthInSquares * gridSize, heightInSquares * gridSize);

            position = findNearestFreeGridCorner(elements, sliderSize, gridSize, padding, menuBarHeight);
        } else {
            position = findFreePosition(elements, sliderSize, padding, 10.0f, 10.0f, 25.0f);
        }

        if (position.x == -1.0f && position.y == -1.0f) { // No free position found
            playBeep();
            position = ImVec2(0.0f, menuBarHeight); // Default to the top-left corner
        }

        sliderSize = ImVec2(200.0f, 20.0f); // Fixed size for the slider

        if (ImGui::Button("Add")) {
            addElementToActiveTemplate(new Slider<int>(
                    label,
                    position,
                    sliderSize,
                    minValue,
                    maxValue,
                    initialValue
            ));
            ImGui::CloseCurrentPopup();
        }

        ImGui::SameLine();
        if (ImGui::Button("Cancel")) {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}

void ConfigurationMode::createFloatSliderSettings() {
    if (ImGui::Button("Add Float Slider")) {
        ImGui::OpenPopup("Add Slider Popup");
    }

    if (ImGui::BeginPopup("Add Slider Popup")) {
        static char label[128] = "Slider (float)";
        static ImVec2 position = ImVec2(100.0f, 100.0f);
        static float minValue = 0.0f;
        static float maxValue = 1.0f;
        static float initialValue = 0.0f;

        ImGui::InputText("Label", label, IM_ARRAYSIZE(label));
        ImGui::InputFloat("Min Value", &minValue);
        ImGui::InputFloat("Max Value", &maxValue);
        ImGui::InputFloat("Initial Value", &initialValue);

        if (initialValue < minValue) initialValue = minValue;
        if (initialValue > maxValue) initialValue = maxValue;

        // Calculate free position for the float slider
        auto elements = templateManager.getActiveTemplateElements();
        ImVec2 sliderSize(200.0f, 20.0f); // Fixed size for the float slider
        ImVec2 padding(10.0f, 10.0f); // Define padding to maintain space between elements

        if (isSnapping) {
            int widthInSquares = ceil(sliderSize.x / gridSize);
            int heightInSquares = ceil(sliderSize.y / gridSize);
            sliderSize = ImVec2(widthInSquares * gridSize, heightInSquares * gridSize);

            position = findNearestFreeGridCorner(elements, sliderSize, gridSize, padding, menuBarHeight);
        } else {
            position = findFreePosition(elements, sliderSize, padding, 10.0f, 10.0f, 25.0f);
        }

        if (position.x == -1.0f && position.y == -1.0f) { // No free position found
            playBeep();
            position = ImVec2(0.0f, menuBarHeight); // Default to the top-left corner
        }

        sliderSize = ImVec2(200.0f, 20.0f); // Fixed size for the slider

        if (ImGui::Button("Add")) {
            addElementToActiveTemplate(new Slider<float>(
                    label,
                    position,
                    sliderSize,
                    minValue,
                    maxValue,
                    initialValue
            ));
            ImGui::CloseCurrentPopup();
        }

        ImGui::SameLine();
        if (ImGui::Button("Cancel")) {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}

void ConfigurationMode::addElementToActiveTemplate(Element* element) {
    templateManager.addElementToActiveTemplate(element);
}
