#include "Element.h"

ImVec2 Element::getPosition() const {
    return position;
}

std::string Element::getLabel() const {
    return label;
}

void Element::setPosition(const ImVec2 &newPos) {
    position = newPos;
}

void Element::setLabel(const std::string &newLabel) {
    label = newLabel;
}

void Element::move(const ImVec2& delta) {
    position.x += delta.x;
    position.y += delta.y;
}

bool Element::getPendingDelete() const {
    return pendingDelete;
}
bool Element::getPendingEdit() const {
    return pendingEdit;
}
bool Element::getPendingChooseWhatToDo() const {
    return pendingChooseWhatToDo;
}

void Element::setPendingDelete(bool newPendingDelete) {
    pendingDelete = newPendingDelete;
}
void Element::setPendingEdit(bool newPendingEdit) {
    pendingEdit = newPendingEdit;
}
void  Element::setPendingChooseWhatToDo(bool newPendingChooseWhatToDo) {
    pendingChooseWhatToDo = newPendingChooseWhatToDo;
}

ImVec2 Element::getPopupPosition() {
    return popupPosition;
}

void Element::setPopupPosition(ImVec2 newPopupPosition) {
    popupPosition = newPopupPosition;
}

void Element::detectRightClick() {
    popupPosition = ImGui::GetMousePos();
    pendingChooseWhatToDo = true; // Mark that the element is pending deletion
}

int Element::getZIndex() const {
    return zIndex;
}

void Element::setZIndex(int z) {
    zIndex = z;
}

void Element::setWasDragged(bool value) {
    wasDragged = value;
}

bool Element::getWasDragged() const {
    return wasDragged;
}
void Element::setModuleName(const std::string &name) {
    moduleName = name;

}
void Element::setModuleID(int id) {
    moduleID = id;
}
std::string Element::getModuleName() const {
    return moduleName;
}
void Element::setSize(const ImVec2 &newSize) {
    size = newSize;
}

ImVec2 Element::getSize() const {
    return size;
}

void Element::to_json(nlohmann::json &j) const {
    j["label"] = label;
    j["position"] = {position.x, position.y};
}

void Element::from_json(const nlohmann::json &j, ImVec2 resolution) {
    if (j.contains("label") && j["label"].is_string()) {
        label = j["label"];
    } else {
        label = "";
    }

    if (j.contains("moduleName") && j["moduleName"].is_string()) {
        moduleName = j["moduleName"];
    } else {
        moduleName = "";
    }

    if (j.contains("size") && j["size"].is_array() && j["size"].size() == 2) {
        size.x = j["size"][0];
        size.y = j["size"][1];
    } else {
        size = ImVec2(0.0f, 0.0f);
    }

    if (j.contains("position") && j["position"].is_array() && j["position"].size() == 2) {
        position.x = j["position"][0];
        position.y = j["position"][1];
    } else {
        position = ImVec2(0.0f, 0.0f);
    }

    ImVec2 scale = Element::getScaleFactors(resolution);

    position = ImVec2(position.x * scale.x, position.y * scale.y);
    size = ImVec2(size.x * scale.x, size.y * scale.y);
}

void Element::setConfigurationMode(bool newBool) {
    configurationMode = newBool;
}


ImVec2 Element::getScaleFactors(ImVec2 templateResolution) {
    GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* videoMode = glfwGetVideoMode(primaryMonitor);

    int monitorWidth = videoMode->width;
    int monitorHeight = videoMode->height;

    float scaleX = monitorWidth / templateResolution.x;
    float scaleY = monitorHeight / templateResolution.y;

    return ImVec2(scaleX, scaleY);
}






