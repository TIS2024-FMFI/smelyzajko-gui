#include <iostream>
#include "OperatingMode.h"
#include "../TestModules/MapModule.h"
#include "../TestModules/CounterModule.h"
#include "../TestModules/UltrasonicModule.h"

int OperatingMode::run() {
    MapModule mapModule = MapModule(&moduleManager);
    CounterModule counterModule = CounterModule(&moduleManager);
    UltrasonicModule ultrasonicModule = UltrasonicModule(&moduleManager);


    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return 1;
    }
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        shortcutsManager.processShortcuts();

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

        if (!templateManager.getActiveTemplateModules().empty()) {
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

    for (GraphicModule* module : templateManager.getActiveTemplateModules()) {
        delete module;
    }
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
        element->setConfigurationMode(false);
        // Draw the element
        element->draw(io);

        if (auto value = element->getBoolValue()){
            moduleManager.setValueFromInputElements(element->getModuleName(),element->getLabel(),*value);
        }

        if (auto value = element->getStringValue()){
            moduleManager.setValueFromInputElements(element->getModuleName(),element->getLabel(),*value);
        }
        if (auto value = element->getIntValue()){
            moduleManager.setValueFromInputElements(element->getModuleName(),element->getLabel(),*value);
        }
        if (auto value = element->getFloatValue()){
            moduleManager.setValueFromInputElements(element->getModuleName(),element->getLabel(),*value);
        }



        ImGui::PopID();
    }
}

void OperatingMode::setupShortcuts() {
    shortcutsManager.setWindow(window);

    shortcutsManager.registerShortcut("Ctrl+Left", [this]() {
        switchTemplate(-1);
    });

    shortcutsManager.registerShortcut("Ctrl+Right", [this]() {
        switchTemplate(1);
    });

    shortcutsManager.registerShortcut("Cmd+Left", [this]() {
        switchTemplate(-1);
    });

    shortcutsManager.registerShortcut("Cmd+Right", [this]() {
        switchTemplate(1);
    });
}

void OperatingMode::switchTemplate(int direction) {
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
