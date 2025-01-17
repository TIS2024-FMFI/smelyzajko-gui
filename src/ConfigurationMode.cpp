#include <iostream>
#include "ConfigurationMode.h"
#include "../TestModules/MapModule.h"
#include "../TestModules/CounterModule.h"
//#include "TemplateManager.h"
#include "../ImGuiFileDialog/ImGuiFileDialog.h"
#include "../ImGuiFileDialog/ImGuiFileDialogConfig.h"
#include "Template.h"

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

int ConfigurationMode::run() {
    MapModule mapModule = MapModule(&moduleManager);
    CounterModule counterModule = CounterModule(&moduleManager);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        shortcutsManager.processShortcuts();
        setupMenuBar();
        processFileDialog();

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
    auto &activeElements = templateManager.getActiveTemplateElements();

    // First, draw the rectangles
    for (int i = 0; i < activeElements.size(); i++) {
        Element *element = activeElements[i];
        ImGui::PushID(i);

        // Draw the element
        element->setConfigurationMode(true);
        element->draw(io);

        ImGui::PopID();
    }

    handleClicksOnElements(activeElements);
}

void ConfigurationMode::drawElementsWithSnappingOn() {
    auto &activeElements = templateManager.getActiveTemplateElements();
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

    handleClicksOnElements(activeElements);
}

void ConfigurationMode::handleElementClick(Element *element,int i) {
    if (element->getPendingChooseWhatToDo()) {
        ImGui::SetNextWindowPos(element->getPopupPosition(), ImGuiCond_Always);
        ImGui::OpenPopup("Element Options");

        if (ImGui::BeginPopupModal("Element Options", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::Text("What would you like to do with this element?");
            if (ImGui::Button("Delete")) {
                element->setPendingDelete(true);
                element->setPendingChooseWhatToDo(false);
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button("Edit")) {
                element->setPendingEdit(true);
                element->setPendingChooseWhatToDo(false);
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button("Cancel")) {
                element->setPendingChooseWhatToDo(false);
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
    }
    else if (element->getPendingDelete()){
        ImGui::SetNextWindowPos(element->getPopupPosition(), ImGuiCond_Always);
        ImGui::OpenPopup("Delete Confirmation");
        if (ImGui::BeginPopupModal("Delete Confirmation", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::Text("Delete this element?");
            if (ImGui::Button("Yes")) {
                templateManager.removeElementFromActiveTemplate(i);
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button("No")) {
                element->setPendingDelete(false);
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
    }
    else if (element->getPendingEdit()) {
        ImGui::SetNextWindowPos(element->getPopupPosition(), ImGuiCond_Always);
        ImGui::OpenPopup("Edit Element Settings");
        if (ImGui::BeginPopupModal("Edit Element Settings", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::Text("Settings for element: %s", element->getLabel().c_str());
            auto settings = element->getSettings();
            for (auto& setting : settings) {
                std::visit([&](auto&& value) {
                    using T = std::decay_t<decltype(value)>;
                    if constexpr (std::is_same_v<T, bool>) {
                        bool val = value;
                        if (ImGui::Checkbox(setting.name.c_str(), &val)) {
                            setting.setter(val);
                        }
                    } else if constexpr (std::is_same_v<T, int>) {
                        int val = value;
                        if (ImGui::InputInt(setting.name.c_str(), &val)) {
                            setting.setter(val);
                        }
                    } else if constexpr (std::is_same_v<T, float>) {
                        float val = value;
                        if (ImGui::InputFloat(setting.name.c_str(), &val)) {
                            setting.setter(val);
                        }
                    } else if constexpr (std::is_same_v<T, std::string>) {
                        char buffer[256];
                        std::strncpy(buffer, value.c_str(), sizeof(buffer));
                        if (ImGui::InputText(setting.name.c_str(), buffer, sizeof(buffer))) {
                            setting.setter(std::string(buffer));
                        }
                    }
                }, setting.value);
            }
            if (ImGui::Button("Close")) {
                element->setPendingEdit(false);
                ImGui::CloseCurrentPopup();

            }
            ImGui::EndPopup();
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
                saveTemplate();
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
            } else if (gridSize > maxGridValue) {
                gridSize = maxGridValue;
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Configuration")) {
//            for (Module& module : modules) {
//                if (ImGui::BeginMenu(module.moduleName.c_str())) {
//                    // Show Graphics and Text parts as separate items
//                    if (ImGui::BeginMenu("Graphics")) {
//                        // Open Settings Popup for Graphics part
//                        std::string popupName = std::string(module.moduleName) + " Graphics Settings";
//                        if (ImGui::Button("Settings")) {
//                            ImGui::OpenPopup(popupName.c_str());
//                        }
//                        renderSettingsPopup(module, "Graphics");
//                        ImGui::EndMenu();
//                    }
//
//                    if (ImGui::BeginMenu("Text")) {
//                        // Open Settings Popup for Text part
//                        std::string popupName = std::string(module.moduleName) + " Text Settings";
//                        if (ImGui::Button("Settings")) {
//                            ImGui::OpenPopup(popupName.c_str());
//                        }
//                        renderSettingsPopup(module, "Text");
//                        ImGui::EndMenu();
//                    }
//
//                    ImGui::EndMenu();
//                }
//            }
            ImGui::EndMenu();
        }

    }


    if (ImGui::BeginMenu("Add Text Label")) {
        createLabelSettings();
        ImGui::EndMenu();
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

    if (ImGui::BeginMenu("Add Modules")) {
        auto elements = templateManager.getActiveTemplateElements();
        for (Module* module : moduleManager.getModules()) {
            if (ImGui::BeginMenu(module->getModuleName().c_str())) {
                int enumerateGraphic = 0;
                for (const std::string& graphicElement : module->getPossibleGraphicsElement()) {
                    if (ImGui::MenuItem(graphicElement.c_str())) {
                        ImVec2 elementSize(300.0f, 200.0f);
                        ImVec2 padding(30.0f, 30.0f);
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
                        //addElementToActiveTemplate(new class Rectangle("Rectangle", position, elementSize));
                        class Rectangle* tempt = new class Rectangle(graphicElement, position, elementSize);
                        tempt->setModuleID(module->getModuleID());
                        tempt->setGraphicElementId(enumerateGraphic);
                        tempt->setModuleName(module->getModuleName());
                        tempt->setGraphicElementName(graphicElement);
                        addElementToActiveTemplate(tempt);
                    }
                    enumerateGraphic++;
                }
                ImGui::EndMenu();
            }
        }
        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Add Input Elements")) {
        auto elements = templateManager.getActiveTemplateElements();

        static const std::unordered_map<std::string, std::function<void(std::string, std::string)>> elementCreators = {
                {"horizontal-slider-int", [&](std::string elementName, std::string moduleName) { createIntSliderSettings(elementName, moduleName); }},
                {"horizontal-slider-float", [&](std::string elementName, std::string moduleName) { createFloatSliderSettings(elementName, moduleName); }},
                {"vertical-slider-int", [&](std::string elementName, std::string moduleName) { createIntSliderSettings(elementName, moduleName); }},
                {"vertical-slider-float", [&](std::string elementName, std::string moduleName) { createFloatSliderSettings(elementName, moduleName); }},
                {"button", [&](std::string elementName, std::string moduleName) { createButton(elementName, moduleName); }},
                {"checkbox", [&](std::string elementName, std::string moduleName) { createCheckbox(elementName, moduleName); }},
                {"text-input", [&](std::string elementName, std::string moduleName) { createTextInput(elementName, moduleName); }},
        };

        for (Module* module : moduleManager.getModules()) {
            std::string moduleName = module->getModuleName();

            if (ImGui::BeginMenu(moduleName.c_str())) {
                for (const auto& group : module->getPossibleInputElements()) {
                    if (ImGui::BeginMenu(group.first.c_str())) {
                        std::string elementType = group.first;

                        for (const auto& elementName : group.second) {

                            auto it = elementCreators.find(elementType);
                            if (it != elementCreators.end()) {
                                it->second(elementName, moduleName);
                            } else {
                                std::cerr << "Unknown element type: " << elementType << std::endl;
                            }
                        }

                        ImGui::EndMenu();
                    }
                }
                ImGui::EndMenu();
            }
        }
        ImGui::EndMenu();
    }

    ImGui::EndMainMenuBar();
}

//
//void ConfigurationMode::renderSettingsPopup(Module& module, const std::string& part) {
//    std::string popupName = std::string(module.moduleName) + " " + part + " Settings";
//
//    // Set the size and position of the popup to be centered on the screen
//    ImGui::SetNextWindowSize(ImVec2(400, 300), ImGuiCond_Always);  // Set size of the popup
//    ImVec2 windowSize = ImGui::GetIO().DisplaySize; // Get screen dimensions
//    ImVec2 popupPos = ImVec2(windowSize.x / 2 - 200, windowSize.y / 2 - 150);  // Centering position
//    ImGui::SetNextWindowPos(popupPos, ImGuiCond_Always);  // Set the position of the popup
//
//    // Begin the modal popup
//    if (ImGui::BeginPopupModal(popupName.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
//        // Center the content inside the popup
//        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(20, 20));  // Add padding for a better layout
//        ImGui::Text("Settings for %s", popupName.c_str());
//
//        // Frequency Slider and Logging Checkbox
//        if (part == "Graphics") {
//            float frequency = module.GetGraphicsFrequency();
//            int frequencyInt = static_cast<int>(frequency);  // Convert to integer for the slider
//            if (ImGui::SliderInt("Frequency", &frequencyInt, 0, 100)) {
//                module.SetGraphicsFrequency(static_cast<float>(frequencyInt)); // Set integer as frequency
//            }
//
//            bool logEnabled = module.IsGraphicsLoggingEnabled();
//            if (ImGui::Checkbox("Enable Logging", &logEnabled)) {
//                module.SetGraphicsLoggingEnabled(logEnabled);
//            }
//        } else if (part == "Text") {
//            float frequency = module.GetTextFrequency();
//            int frequencyInt = static_cast<int>(frequency);  // Convert to integer for the slider
//            if (ImGui::SliderInt("Frequency", &frequencyInt, 0, 100)) {
//                module.SetTextFrequency(static_cast<float>(frequencyInt)); // Set integer as frequency
//            }
//
//            bool logEnabled = module.IsTextLoggingEnabled();
//            if (ImGui::Checkbox("Enable Logging", &logEnabled)) {
//                module.SetTextLoggingEnabled(logEnabled);
//            }
//        }
//
//        // Apply and Cancel buttons
//        ImGui::NewLine();
//        if (ImGui::Button("Apply")) {
//            ImGui::CloseCurrentPopup();
//        }
//
//        ImGui::SameLine();
//        if (ImGui::Button("Cancel")) {
//            ImGui::CloseCurrentPopup();
//        }
//
//        ImGui::PopStyleVar();  // Restore the style to default
//        ImGui::EndPopup();
//    }
//}

void ConfigurationMode::drawGrid() const {
    ImVec2 displaySize = io.DisplaySize;
    ImDrawList *drawList = ImGui::GetBackgroundDrawList();

    ImU32 whiteColor = IM_COL32_WHITE;

    for (float x = 0; x < displaySize.x; x += gridSize) {
        drawList->AddLine(ImVec2(x, menuBarHeight), ImVec2(x, displaySize.y), whiteColor);
    }

    for (float y = menuBarHeight; y < displaySize.y; y += gridSize) {
        drawList->AddLine(ImVec2(0, y), ImVec2(displaySize.x, y), whiteColor);
    }
}

void ConfigurationMode::bringElementToTop(std::vector<Element*>& elements, Element* element) {
    if (isAnyPendingElement(elements)) {
        return;
    }

    auto it = std::find(elements.begin(), elements.end(), element);

    if (it != elements.end()) {
        // Remove the element from its current position
        elements.erase(it);

        // Add the element to the end (top of the z-index)
        elements.push_back(element);
    }

    // Update z-indices for all elements
    for (int i = 0; i < elements.size(); ++i) {
        elements[i]->setZIndex(i);
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


void ConfigurationMode::createIntSliderSettings(std::string elementName, std::string moduleName) {
    std::string popupName = elementName + "-" + moduleName;

    if (ImGui::Button(elementName.c_str())) {
        ImGui::OpenPopup(popupName.c_str());
    }

    if (ImGui::BeginPopup(popupName.c_str())) {
        static char label[128];
        std::strncpy(label, elementName.c_str(), sizeof(label) - 1);
        label[sizeof(label) - 1] = '\0';
        static ImVec2 position;
        static int minValue = 0;
        static int maxValue = 10;
        static int initialValue = 5;
        static bool isHorizontal = true;

        ImGui::InputText("Label", label, IM_ARRAYSIZE(label));
        ImGui::InputInt("Min Value", &minValue);
        ImGui::InputInt("Max Value", &maxValue);
        ImGui::InputInt("Initial Value", &initialValue);

        if (initialValue < minValue) initialValue = minValue;
        if (initialValue > maxValue) initialValue = maxValue;

        ImGui::Checkbox("Horizontal Slider", &isHorizontal);

        auto elements = templateManager.getActiveTemplateElements();
        ImVec2 sliderSize = isHorizontal ? ImVec2(200.0f, 20.0f) : ImVec2(20.0f, 200.0f);
        ImVec2 padding(10.0f, 10.0f);

        if (isSnapping) {
            int widthInSquares = ceil(sliderSize.x / gridSize);
            int heightInSquares = ceil(sliderSize.y / gridSize);
            sliderSize = ImVec2(widthInSquares * gridSize, heightInSquares * gridSize);

            position = findNearestFreeGridCorner(elements, sliderSize, gridSize, padding, menuBarHeight);
        } else {
            position = findFreePosition(elements, sliderSize, padding, 10.0f, 10.0f, 25.0f);
        }

        if (position.x == -1.0f && position.y == -1.0f) {
            playBeep();
            position = ImVec2(0.0f, menuBarHeight);
        }

        if (ImGui::Button("Add")) {
            if (isHorizontal) {
                auto *element = new HorizontalSlider<int>(
                        label,
                        position,
                        sliderSize,
                        minValue,
                        maxValue,
                        initialValue
                );
                setNewElementAndAddToActiveTemplate(element, label, moduleName, position, sliderSize);
            } else {
                auto *element = new VerticalSlider<int>(
                        label,
                        position,
                        sliderSize,
                        minValue,
                        maxValue,
                        initialValue
                );
                setNewElementAndAddToActiveTemplate(element, label, moduleName, position, sliderSize);
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

void ConfigurationMode::createFloatSliderSettings(std::string elementName, std::string moduleName) {
    std::string popupName = elementName + "-" + moduleName;

    if (ImGui::Button(elementName.c_str())) {
        ImGui::OpenPopup(popupName.c_str());
    }

    if (ImGui::BeginPopup(popupName.c_str())) {
        static char label[128];
        std::strncpy(label, elementName.c_str(), sizeof(label) - 1);
        label[sizeof(label) - 1] = '\0';
        static ImVec2 position = ImVec2(100.0f, 100.0f);
        static float minValue = 0.0f;
        static float maxValue = 1.0f;
        static float initialValue = 0.0f;
        static bool isHorizontal = true;

        ImGui::InputText("Label", label, IM_ARRAYSIZE(label));
        ImGui::InputFloat("Min Value", &minValue);
        ImGui::InputFloat("Max Value", &maxValue);
        ImGui::InputFloat("Initial Value", &initialValue);

        if (initialValue < minValue) initialValue = minValue;
        if (initialValue > maxValue) initialValue = maxValue;

        ImGui::Checkbox("Horizontal Slider", &isHorizontal);

        auto elements = templateManager.getActiveTemplateElements();
        ImVec2 sliderSize = isHorizontal ? ImVec2(200.0f, 20.0f) : ImVec2(20.0f, 200.0f);
        ImVec2 padding(10.0f, 10.0f);

        if (isSnapping) {
            int widthInSquares = ceil(sliderSize.x / gridSize);
            int heightInSquares = ceil(sliderSize.y / gridSize);
            sliderSize = ImVec2(widthInSquares * gridSize, heightInSquares * gridSize);

            position = findNearestFreeGridCorner(elements, sliderSize, gridSize, padding, menuBarHeight);
        } else {
            position = findFreePosition(elements, sliderSize, padding, 10.0f, 10.0f, 25.0f);
        }

        if (position.x == -1.0f && position.y == -1.0f) {
            playBeep();
            position = ImVec2(0.0f, menuBarHeight);
        }

        if (ImGui::Button("Add")) {
            if (isHorizontal) {
                auto *element = new HorizontalSlider<float>(
                        label,
                        position,
                        sliderSize,
                        minValue,
                        maxValue,
                        initialValue
                );
                setNewElementAndAddToActiveTemplate(element, label, moduleName, position, sliderSize);
            } else {
                auto *element = new VerticalSlider<float>(
                        label,
                        position,
                        sliderSize,
                        minValue,
                        maxValue,
                        initialValue
                );
                setNewElementAndAddToActiveTemplate(element, label, moduleName, position, sliderSize);
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


void ConfigurationMode::addElementToActiveTemplate(Element* element) {
    templateManager.addElementToActiveTemplate(element);
}
void ConfigurationMode::addModuleToActiveTemplate(GraphicModule* graphicModule) {
    templateManager.addModuleToActiveTemplate(graphicModule);
}

bool ConfigurationMode::isAnyPendingElement(std::vector<Element *> &elements) {
    for (const auto& elem : elements) {
        if (elem->getPendingChooseWhatToDo() || elem->getPendingDelete() || elem->getPendingEdit()) {
            return true;
        }
    }
    return false;
}

void ConfigurationMode::handleClicksOnElements(std::vector<Element*>& elements) {
    // Check for clicks and move the element to top
    Element *clickedElement = nullptr;
    for (int i = elements.size() - 1; i >= 0; i--) {
        Element *element = elements[i];
        ImRect bbox = element->getBoundingBox();

        if (bbox.Contains(ImGui::GetMousePos())) {
            if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
                clickedElement = element;
                break;
            }
        }
    }

    bool pendingPopupForAnyElement = isAnyPendingElement(elements);
    // If an element is clicked, bring it to the top
    if (clickedElement) {
        bringElementToTop(elements, clickedElement);
    }


    bool clickHandled = false;
    for (int i = elements.size() - 1; i >= 0; i--) {
        Element *element = elements[i];
        ImGui::PushID(i);

        element->handleClicks(io);

        ImRect bbox = element->getBoundingBox();
        // Detect right click on element
        if (!pendingPopupForAnyElement) {
            if (bbox.Contains(ImGui::GetMousePos())) {
                if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
                    element->detectRightClick();
                    clickHandled = true;
                }
            }
        }

        handleElementClick(element,i);

        ImGui::PopID();

        if (clickHandled) {
            break;
        }
    }
}

void ConfigurationMode::setupShortcuts() {
    shortcutsManager.setWindow(window);

    shortcutsManager.registerShortcut("Ctrl+Q", [this]() {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    });

    shortcutsManager.registerShortcut("Ctrl+S", [this]() {
        saveTemplate();
    });
}

void ConfigurationMode::saveTemplate() {
    // Check if a file dialog is already open
    if (ImGuiFileDialog::Instance()->IsOpened("ChooseFileDlgKey")) {
        return; // If it's already open, return and avoid opening it again
    }

    IGFD::FileDialogConfig config;
    config.path = "../templates"; // Default path for the file dialog
    if (!templateManager.getActiveTemplateName().empty()) {
        config.fileName = templateManager.getActiveTemplateName() + ".json";
    }

    // Open the file dialog
    ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".json", config);
}

void ConfigurationMode::processFileDialog() {
    // Display the file dialog and handle the file selection
    if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey")) {
        if (ImGuiFileDialog::Instance()->IsOk()) {
            std::filesystem::path filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
            try {
                bool isNew = false;
                std::string templateName = templateManager.getActiveTemplate().getName();
                std::string fileName = filePathName.filename().string();

                // Remove ".json" extension if present
                if (fileName.size() > 5 && fileName.substr(fileName.size() - 5) == ".json") {
                    fileName = fileName.substr(0, fileName.size() - 5);
                }

                // Check if it's a new template or if the name doesn't match
                if (templateName.empty() || templateName != fileName) {
                    isNew = true;
                }

                if (isNew) {
                    templateManager.getActiveTemplate().saveTemplate(filePathName, fileName);
                    Template newTemplate = Template("../templates/" + filePathName.filename().string(), true);
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

        // Close the file dialog after file is selected or canceled
        ImGuiFileDialog::Instance()->Close();
    }
}

void ConfigurationMode::setNewElementAndAddToActiveTemplate(Element *element, std::string elementName, std::string moduleName, ImVec2 position, ImVec2 elementSize) {
    element->setConfigurationMode(true);
    element->setLabel(elementName);
    element->setModuleName(moduleName);
    element->setPosition(position);
    element->setSize(elementSize);
    addElementToActiveTemplate(element);
}

void ConfigurationMode::createButton(std::string elementName, std::string moduleName) {
    if (ImGui::MenuItem(elementName.c_str())) {
        ImVec2 elementSize(100.0f, 25.0f);
        if (isSnapping) {
            elementSize = ImVec2(gridSize, gridSize);
        }

        ImVec2 position = getPosition(elementSize);

        auto element = new Button();
        setNewElementAndAddToActiveTemplate(element, elementName, moduleName, position, elementSize);
    }
}

void ConfigurationMode::createCheckbox(std::string elementName, std::string moduleName) {
    if (ImGui::MenuItem(elementName.c_str())) {
        ImVec2 elementSize(30.0f, 30.0f);
        if (isSnapping) {
            elementSize = ImVec2(gridSize, gridSize);
        }

        ImVec2 position = getPosition(elementSize);

        auto element = new Checkbox();
        setNewElementAndAddToActiveTemplate(element, elementName, moduleName, position, elementSize);
    }
}

void ConfigurationMode::createTextInput(std::string elementName, std::string moduleName) {
    if (ImGui::MenuItem(elementName.c_str())) {
        ImVec2 elementSize(250.0f, 30.0f);
        if (isSnapping) {
            elementSize = ImVec2(gridSize, gridSize);
        }

        ImVec2 position = getPosition(elementSize);

        auto element = new TextInput();
        setNewElementAndAddToActiveTemplate(element, elementName, moduleName, position, elementSize);
    }
}

ImVec2 ConfigurationMode::getPosition(ImVec2 elementSize) {
    ImVec2 position;
    ImVec2 padding(10.0f, 10.0f);
    auto elements = templateManager.getActiveTemplateElements();

    if (isSnapping) {
        position = findNearestFreeGridCorner(elements, elementSize, gridSize, padding, menuBarHeight);
    } else {
        position = findFreePosition(elements, elementSize, padding, 20.0f, 20.0f, menuBarHeight);
    }

    if (position.x == -1.0f && position.y == -1.0f) { // No free position found
        playBeep();
        position = ImVec2(0.0f, menuBarHeight);
    }
    return position;
}
