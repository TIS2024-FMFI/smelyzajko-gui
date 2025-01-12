#include <iostream>
#include "ConfigurationMode.h"
#include "widgets/Rectangle.h"
#include "widgets/Checkbox.h"
#include "widgets/Button.h"
#include "widgets/Slider.h"
#include "widgets/SingleLineLabel.h"
#include "widgets/MultiLineLabel.h"

//// Example modules for demonstration
//std::vector<Module> modules = {
//        Module(1, "Map"),
//        Module(2, "Lidar"),
//        Module(3, "Sinusoid"),
//};

inline bool isOverlapping(const ImRect& a, const ImRect& b) {
    return !(a.Max.x <= b.Min.x ||  // No overlap on the left
             a.Min.x >= b.Max.x ||  // No overlap on the right
             a.Max.y <= b.Min.y ||  // No overlap above
             a.Min.y >= b.Max.y);   // No overlap below
}

ImVec2 findFreePosition(const std::vector<Element*>& elements, const ImVec2& elementSize, const ImVec2& start = ImVec2(10.0f, 10.0f), float step = 10.0f, float padding = 15.0f, float menuHeight = 20.0f) {
    ImVec2 position = start;
    bool isPositionFree;

    do {
        isPositionFree = true;
        ImRect newBoundingBox(
                ImVec2(position.x - padding, position.y - padding),
                ImVec2(position.x + elementSize.x + padding, position.y + elementSize.y + padding)
        );

        // Check overlap with existing elements
        for (const auto& element : elements) {
            if (isOverlapping(newBoundingBox, element->getBoundingBox())) {
                isPositionFree = false;
                position.x += step; // Move horizontally
                if (position.x + elementSize.x + padding > ImGui::GetIO().DisplaySize.x) {
                    position.x = start.x; // Reset X
                    position.y += step;  // Move vertically
                }
                break;
            }
        }

        // Check if position is above the reserved main menu height
        if (isPositionFree && position.y < menuHeight + padding) {
            isPositionFree = false;
            position.y = menuHeight + padding; // Move below the menu area
        }

    } while (!isPositionFree && position.y + elementSize.y + padding <= ImGui::GetIO().DisplaySize.y);

    return position ;
}


int ConfigurationMode::run() {
    io = ImGui::GetIO();
    (void)io;

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        setupMenuBar();

        ImGui::SetNextWindowPos(ImVec2(0, 0)); // Top-left corner of the screen
        ImGui::SetNextWindowSize(io.DisplaySize); // Fullscreen size

        ImGui::Begin("Main Window", nullptr,
                     ImGuiWindowFlags_NoTitleBar |
                     ImGuiWindowFlags_NoCollapse |
                     ImGuiWindowFlags_NoResize |
                     ImGuiWindowFlags_NoMove |
                     ImGuiWindowFlags_NoBringToFrontOnFocus |
                     ImGuiWindowFlags_NoScrollbar
        );


            drawElements();

        ImGui::End();

        if (showGrid) drawGrid();

        // render all toast notifications
        toastManager.renderNotifications();

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

void ConfigurationMode::setupMenuBar() {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Open")) {
            }
            if (ImGui::MenuItem("Save")) {
            }
            if (ImGui::MenuItem("Exit")) {
                glfwSetWindowShouldClose(window, GLFW_TRUE);
            }
            ImGui::EndMenu();
        }
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

            ImGui::InputFloat("Grid Size", &gridSize, 0.1f, 1.0f);

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

        if (ImGui::BeginMenu("Controls")) {
            auto elements = templateManager.getActiveTemplateElements();

            if (ImGui::MenuItem("Add Rectangle")) {
                ImVec2 elementSize(300.0f, 200.0f); // Fixed size for the rectangle (Width:Height = 2:3)
                ImVec2 padding(30.0f, 30.0f); // Define padding to maintain space between elements
                ImVec2 position = findFreePosition(elements, elementSize, padding, 20.0f, 20.0f, 25.0f);
                addElementToActiveTemplate(new Rectangle("Rectangle", position, elementSize));
            }

            if (ImGui::MenuItem("Add Checkbox")) {
                ImVec2 elementSize(30.0f, 30.0f); // Define size for checkbox
                ImVec2 position = findFreePosition(elements, elementSize);
                addElementToActiveTemplate(new Checkbox("Checkbox", position, false));
            }

            if (ImGui::MenuItem("Add Button")) {
                ImVec2 buttonSize(100.0f, 25.0f); // Fixed size for the button
                ImVec2 padding(20.0f, 20.0f); // Define padding to maintain space between elements
                ImVec2 position = findFreePosition(elements, buttonSize, padding, 20.0f, 20.0f, 25.0f);
                addElementToActiveTemplate(new Button("Button", position, buttonSize));
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

void ConfigurationMode::renderSettingsPopup(Module& module, const std::string& part) {
    std::string popupName = std::string(module.moduleName) + " " + part + " Settings";

    // Set the size and position of the popup to be centered on the screen
    ImGui::SetNextWindowSize(ImVec2(400, 300), ImGuiCond_Always);  // Set size of the popup
    ImVec2 windowSize = ImGui::GetIO().DisplaySize; // Get screen dimensions
    ImVec2 popupPos = ImVec2(windowSize.x / 2 - 200, windowSize.y / 2 - 150);  // Centering position
    ImGui::SetNextWindowPos(popupPos, ImGuiCond_Always);  // Set the position of the popup

    // Begin the modal popup
    if (ImGui::BeginPopupModal(popupName.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        // Center the content inside the popup
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(20, 20));  // Add padding for a better layout
        ImGui::Text("Settings for %s", popupName.c_str());

        // Frequency Slider and Logging Checkbox
        if (part == "Graphics") {
            float frequency = module.GetGraphicsFrequency();
            int frequencyInt = static_cast<int>(frequency);  // Convert to integer for the slider
            if (ImGui::SliderInt("Frequency", &frequencyInt, 0, 100)) {
                module.SetGraphicsFrequency(static_cast<float>(frequencyInt)); // Set integer as frequency
            }

            bool logEnabled = module.IsGraphicsLoggingEnabled();
            if (ImGui::Checkbox("Enable Logging", &logEnabled)) {
                module.SetGraphicsLoggingEnabled(logEnabled);
            }
        } else if (part == "Text") {
            float frequency = module.GetTextFrequency();
            int frequencyInt = static_cast<int>(frequency);  // Convert to integer for the slider
            if (ImGui::SliderInt("Frequency", &frequencyInt, 0, 100)) {
                module.SetTextFrequency(static_cast<float>(frequencyInt)); // Set integer as frequency
            }

            bool logEnabled = module.IsTextLoggingEnabled();
            if (ImGui::Checkbox("Enable Logging", &logEnabled)) {
                module.SetTextLoggingEnabled(logEnabled);
            }
        }

        // Apply and Cancel buttons
        ImGui::NewLine();
        if (ImGui::Button("Apply")) {
            ImGui::CloseCurrentPopup();
        }

        ImGui::SameLine();
        if (ImGui::Button("Cancel")) {
            ImGui::CloseCurrentPopup();
        }

        ImGui::PopStyleVar();  // Restore the style to default
        ImGui::EndPopup();
    }
}

void ConfigurationMode::drawGrid() const {
    ImVec2 displaySize = io.DisplaySize;
    float menuBarHeight = ImGui::GetFrameHeight();
    ImDrawList *drawList = ImGui::GetBackgroundDrawList();

    for (float x = 0; x < displaySize.x; x += gridSize) {
        drawList->AddLine(ImVec2(x, menuBarHeight), ImVec2(x, displaySize.y), IM_COL32(255, 255, 255, 255));
    }

    for (float y = menuBarHeight; y < displaySize.y; y += gridSize) {
        drawList->AddLine(ImVec2(0, y), ImVec2(displaySize.x, y), IM_COL32(255, 255, 255, 255));
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
        position = findFreePosition(elements, labelSize, padding, 10.0f, 10.0f, 25.0f);

        if (ImGui::Button("Add")) {
            if (isMultiLine) {
                addElementToActiveTemplate(new MultiLineLabel(
                        text,
                        position,
                        textSize
                ));
            } else {
                // sanitize text
                for (size_t i = 0; i < strlen(text); ++i) {
                    if (text[i] == '\n') {
                        text[i] = ' '; // Replace newline with space
                    }
                }
                addElementToActiveTemplate(new SingleLineLabel(
                        text,
                        position,
                        textSize
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
    if (ImGui::Button("Add Integer Slider")) {
        ImGui::OpenPopup("Add Int Slider Popup");
    }

    if (ImGui::BeginPopup("Add Int Slider Popup")) {
        static char label[128] = "Slider (int)";
        static ImVec2 position = ImVec2(100.0f, 100.0f);
        static ImVec2 size = ImVec2(200.0f, 20.0f);
        static int minValue = 0;
        static int maxValue = 10;
        static int initialValue = 5;

        ImGui::InputText("Label", label, IM_ARRAYSIZE(label));
        ImGui::InputInt("Min Value", &minValue);
        ImGui::InputInt("Max Value", &maxValue);
        ImGui::InputInt("Initial Value", &initialValue);

        if (initialValue < minValue) initialValue = minValue;
        if (initialValue > maxValue) initialValue = maxValue;

        // Calculate free position for the slider
        auto elements = templateManager.getActiveTemplateElements();
        ImVec2 sliderSize(200.0f, 20.0f); // Fixed size for the slider
        ImVec2 padding(10.0f, 10.0f); // Define padding to maintain space between elements
        position = findFreePosition(elements, sliderSize, padding, 10.0f, 10.0f, 25.0f);

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
        static ImVec2 size = ImVec2(200.0f, 20.0f);
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
        position = findFreePosition(elements, sliderSize, padding, 10.0f, 10.0f, 25.0f);

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
