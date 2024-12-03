#include <iostream>
#include "ConfigurationMode.h"
#include "Rectangle.h"
#include "Checkbox.h"
#include "Button.h"
#include "Slider.h"




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
        if (ImGui::Button("Add Checkbox")) {
            activeElements.emplace_back(new Checkbox("Checkbox " + std::to_string(activeElements.size()), ImVec2(100.0f, 100.0f), false)); // Initial state is unchecked
        }
        if (ImGui::Button("Add Button")) {
            activeElements.emplace_back(new Button("Button " + std::to_string(activeElements.size()), ImVec2(100.0f, 100.0f), ImVec2(100.0f, 25.0f)));
        }
        setupIntSlider();
        setupFloatSlider();
        ImGui::End();
        if (isSnapping){
            drawElementsWithSnappingOn();
        }else{
            drawElements();
        }



        ImGui::End();

        if (showGrid) drawGrid();

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
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

void ConfigurationMode::drawElements() {
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


void ConfigurationMode::drawElementsWithSnappingOn() {
    for (int i = 0; i < activeElements.size(); i++) {
        Element* element = activeElements[i];
        ImGui::PushID(i);

        // Get the element's position
        ImVec2 elementPos = element->getPosition();

        // Handle dragging
        if (ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
            ImVec2 dragDelta = ImGui::GetMouseDragDelta();
            elementPos.x += dragDelta.x;
            elementPos.y += dragDelta.y;

            ImGui::ResetMouseDragDelta(); // Reset delta after applying movement
        }

        // Draw the element
        element->draw(io);

        // On mouse release, snap to nearest grid point
        if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {

            elementPos.x = round(elementPos.x / gridSize) * gridSize ;
            elementPos.y = round((elementPos.y  -  menuBarHeight )/ gridSize) * gridSize  +  menuBarHeight;
            element->setPosition(elementPos);
        }

        // Draw the element after any movement/snapping updates
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



void ConfigurationMode::setupMenuBar() {
    if (ImGui::BeginMainMenuBar()) {
        menuBarHeight = ImGui::GetWindowHeight();
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

            ImGui::InputFloat("Grid Size", &gridSize, 5.0f, 1.0f);

            // Minimum value to avoid dividing by 0
            if (gridSize < minGridValue) {
                gridSize = minGridValue;
            }else if (gridSize > maxGridValue){
                gridSize = maxGridValue;
            }

            ImGui::EndMenu();
        }

    }
    ImGui::EndMainMenuBar();
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
    auto it = std::find(activeElements.begin(), activeElements.end(), element);
    if (it != activeElements.end()) {
        activeElements.erase(it);
        activeElements.push_back(element);
    }

    for (int i = 0; i < activeElements.size(); ++i) {
        activeElements[i]->setZIndex(i);
    }
}

void ConfigurationMode::setupIntSlider() {
    if (ImGui::Button("Add Slider (Int)")) {
        ImGui::OpenPopup("Add Int Slider Popup");
    }

    if (ImGui::BeginPopup("Add Int Slider Popup")) {
        static char label[128] = "Slider (int)";
        static float position[2] = {100.0f, 100.0f};
        static float size[2] = {200.0f, 20.0f};
        static int minValue = 0;
        static int maxValue = 10;
        static int initialValue = 5;

        ImGui::InputText("Label", label, IM_ARRAYSIZE(label));
        ImGui::InputInt("Min Value", &minValue);
        ImGui::InputInt("Max Value", &maxValue);
        ImGui::InputInt("Initial Value", &initialValue);

        if (initialValue < minValue) initialValue = minValue;
        if (initialValue > maxValue) initialValue = maxValue;

        if (ImGui::Button("Add")) {
            activeElements.emplace_back(new Slider<int>(
                    label,
                    ImVec2(position[0], position[1]),
                    ImVec2(size[0], size[1]),
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

void ConfigurationMode::setupFloatSlider() {
    if (ImGui::Button("Add Slider (Float)")) {
        ImGui::OpenPopup("Add Slider Popup");
    }

    if (ImGui::BeginPopup("Add Slider Popup")) {
        static char label[128] = "Slider (float)";
        static float position[2] = {100.0f, 100.0f};
        static float size[2] = {200.0f, 20.0f};
        static float minValue = 0.0f;
        static float maxValue = 1.0f;
        static float initialValue = 0.0f;

        ImGui::InputText("Label", label, IM_ARRAYSIZE(label));
        ImGui::InputFloat("Min Value", &minValue);
        ImGui::InputFloat("Max Value", &maxValue);
        ImGui::InputFloat("Initial Value", &initialValue);

        if (initialValue < minValue) initialValue = minValue;
        if (initialValue > maxValue) initialValue = maxValue;

        if (ImGui::Button("Add")) {
            activeElements.emplace_back(new Slider<float>(
                    label,
                    ImVec2(position[0], position[1]),
                    ImVec2(size[0], size[1]),
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
