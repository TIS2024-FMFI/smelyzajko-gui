#include <iostream>
#include "ConfigurationMode.h"
#include "widgets/Rectangle.h"
#include "widgets/Checkbox.h"
#include "widgets/Button.h"
#include "widgets/Slider.h"
#include "widgets/SingleLineLabel.h"
#include "widgets/MultiLineLabel.h"
#include "OperatingMode.h"
#include "ModuleManager.h"

ModuleManager moduleManager;

int OperatingMode::run() {
    moduleManager.createModules();
    std::cout << "OperatingMode::run()" << std::endl;
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
                     ImGuiWindowFlags_NoTitleBar |    // Remove title bar
                     ImGuiWindowFlags_NoCollapse |   // Prevent collapsing
                     ImGuiWindowFlags_NoResize |     // Disable resizing
                     ImGuiWindowFlags_NoMove |       // Prevent moving the window
                     ImGuiWindowFlags_NoBringToFrontOnFocus | // Prevent window focus changes
                     ImGuiWindowFlags_NoScrollbar    // Disable scrollbar (optional)
        );

        ImGui::Begin("Controls");
        if (ImGui::Button("Add Rectangle")) {
            activeElements.emplace_back(new Rectangle("Rectangle" + std::to_string(activeElements.size()), ImVec2(100.0f, 100.0f), ImVec2(200.0f, 100.0f)));
        }



        ImGui::End();
        drawElements();
        std::cout<<activeElements.size()<<std::endl;
        if (activeElements.size() > 0) {
            ImVec2 posssition =  activeElements[0]->getPosition();
            moduleManager.renderModules(io,posssition);

        }


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

    for (Element* element : activeElements) {
        delete element;
    }
    activeElements.clear();

    cleanupImGui();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
void OperatingMode::setupMenuBar() {
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
//        if (ImGui::BeginMenu("Templates")) {
//            for (const Template& aTemplate : templateHandler.allTemplates) {
//                if (ImGui::MenuItem(aTemplate.name.c_str())) {
//                    templateHandler.setActiveTemplate(aTemplate);
//                }
//            }
//            ImGui::EndMenu();
//        }
        if (ImGui::BeginMenu("Options")) {

            ImGui::Checkbox("Enable Snapping", &isSnapping);
            ImGui::Checkbox("Show Grid", &showGrid);

            ImGui::InputFloat("Grid Size", &gridSize, 0.1f, 1.0f);

            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Configuration")) {

            moduleManager.createModules();
            std::vector<Module*> modules = moduleManager.getModules();

            for (Module* module : modules) {
                if (ImGui::BeginMenu(module->moduleName.c_str())) {
                    // Show Graphics and Text parts as separate items
                    if (ImGui::BeginMenu("Graphics")) {
                        // Open Settings Popup for Graphics part
                        std::string popupName = std::string(module->moduleName) + " Graphics Settings";
                        if (ImGui::Button("Settings")) {
                            ImGui::OpenPopup(popupName.c_str());
                        }
                        renderSettingsPopup(*module, "Graphics");
                        ImGui::EndMenu();
                    }

                    if (ImGui::BeginMenu("Text")) {
                        // Open Settings Popup for Text part
                        std::string popupName = std::string(module->moduleName) + " Text Settings";
                        if (ImGui::Button("Settings")) {
                            ImGui::OpenPopup(popupName.c_str());
                        }
                        renderSettingsPopup(*module, "Text");
                        ImGui::EndMenu();
                    }

                    ImGui::EndMenu();
                }
            }
            ImGui::EndMenu();
        }
    }
    ImGui::EndMainMenuBar();
}

void OperatingMode::renderSettingsPopup(Module& module, const std::string& part) {
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

void OperatingMode::drawElements() {
    // First, draw the rectangles
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
                    activeElements.erase(activeElements.begin() + i);
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

void OperatingMode::bringElementToTop(Element* element) {
    // Remove the clicked element and add it to the end of the list
    auto it = std::find(activeElements.begin(), activeElements.end(), element);
    if (it != activeElements.end()) {
        activeElements.erase(it);
        activeElements.push_back(element);
    }

    for (int i = 0; i < activeElements.size(); ++i) {
        activeElements[i]->setZIndex(i);
    }
}
