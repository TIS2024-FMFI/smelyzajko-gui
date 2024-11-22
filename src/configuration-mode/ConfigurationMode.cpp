#include "ConfigurationMode.h"

int ConfigurationMode::run() {

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        setupMenuBar();

        ImGui::Begin("Controls");
        if (ImGui::Button("Add Window")) {
            activeElementPlaceholders.emplace_back("Window" + std::to_string(activeElementPlaceholders.size() + 1), ImVec2(100, 100), ImVec2(200, 150));
        }
        ImGui::End();

        drawElementPlaceholders();

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    cleanupImGui();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

void ConfigurationMode::drawElementPlaceholders() {
    for (int i = 0; i < activeElementPlaceholders.size(); i++) {
        auto &element = activeElementPlaceholders[i];
        ImGui::PushID(i);
        element.draw();
        ImGui::PopID();

        if (element.pendingDelete) {
            ImGui::SetNextWindowPos(element.deletePopupPosition, ImGuiCond_Always);
            ImGui::OpenPopup("Delete Confirmation");
            if (ImGui::BeginPopupModal("Delete Confirmation", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
                ImGui::Text("Delete this window?");
                if (ImGui::Button("Yes")) {
                    activeElementPlaceholders.erase(activeElementPlaceholders.begin() + i);
                    ImGui::CloseCurrentPopup();
                    ImGui::EndPopup();
                    break;
                }
                ImGui::SameLine();
                if (ImGui::Button("No")) {
                    element.pendingDelete = false;
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndPopup();
            }
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
    }
    ImGui::EndMainMenuBar();
}