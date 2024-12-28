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

#include "../TestModules/MapModule.h"
#include "../TestModules/CounterModule.h"


int OperatingMode::run() {
    CounterModule counterModule = CounterModule(&moduleManager);
    MapModule mapModule = MapModule(&moduleManager);
    io = ImGui::GetIO();
    (void)io;
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return 1;
    }
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
            addElementToActiveTemplate(new Rectangle("Rectangle" , ImVec2(100.0f, 100.0f), ImVec2(200.0f, 100.0f),
                                                      false));
        }


        ImGui::End();
        if (!templateManager.getActiveTemplateModules().empty()){
            moduleManager.setActiveModuleAndDraw(templateManager.getActiveTemplateModules(),io);
        }

        drawElements();
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

//    for (GraphicModule* module : templateManager.getActiveTemplateModules()) {
//        delete module;
//    }
    cleanupImGui();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}


void OperatingMode::setupMenuBar() {
    if (ImGui::BeginMainMenuBar()) {
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


    }
    ImGui::EndMainMenuBar();
}



void OperatingMode::drawElements() {
    // First, draw the rectangles
    auto activeElements = templateManager.getActiveTemplateElements();
    for (int i = 0; i < activeElements.size(); i++) {
        Element *element = activeElements[i];
        ImGui::PushID(i);

        // Draw the element
        element->draw(io);

        ImGui::PopID();
    }
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

void OperatingMode::addElementToActiveTemplate(Element* element) {
    templateManager.addElementToActiveTemplate(element);
}
