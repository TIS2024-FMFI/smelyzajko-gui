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
            activeElements.emplace_back(new Rectangle("Rectangle" + std::to_string(activeElements.size()), ImVec2(100.0f, 100.0f), ImVec2(200.0f, 100.0f),
                                                      false));
        }



        ImGui::End();
        drawElements();
//        std::cout<<activeElements.size()<<std::endl;
        if (activeElements.size() > 0) {

            ImVec2 posssition =  activeElements[0]->getPosition();
            ImVec2 size = activeElements[0]->getSize();
            moduleManager.renderModules(io,posssition,size);

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



void OperatingMode::drawElements() {
    // First, draw the rectangles
    for (int i = 0; i < activeElements.size(); i++) {
        Element *element = activeElements[i];
        ImGui::PushID(i);

        // Draw the element
        element->draw(io);

        ImGui::PopID();
    }
}


