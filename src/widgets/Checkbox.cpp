#include "Checkbox.h"
#include "iostream"

bool Checkbox::isChecked() const {
    return checked;
}

void Checkbox::setChecked(bool state) {
    checked = state;
}

void Checkbox::toggle() {
    checked = !checked;
}

ImRect Checkbox::getBoundingBox() const {
    ImVec2 text_size = ImGui::CalcTextSize(label.c_str());
    ImVec2 checkBoxSize(20.0f, 20.0f);
    ImVec2 totalSize(checkBoxSize.x + text_size.x + 5.0f, std::max(checkBoxSize.y, text_size.y));
    return ImRect(position, ImVec2(position.x + totalSize.x, position.y + totalSize.y));
}

void Checkbox::draw(ImGuiIO &io) {
    ImGui::SetCursorScreenPos(position);
    ImGui::SetNextItemAllowOverlap();
    if (ImGui::Checkbox(label.c_str(), &checked)) {}
}

void Checkbox::handleClicks(ImGuiIO &io) {
    ImRect bbox = getBoundingBox();
    ImGui::SetCursorScreenPos(position);
    ImGui::InvisibleButton(("Checkbox" + label).c_str(), bbox.GetSize());
    if (ImGui::IsItemActive() && ImGui::IsMouseDragging(0)) {
        ImVec2 delta = io.MouseDelta;
        position.x += delta.x;
        position.y += delta.y;
    }
    if (ImGui::IsItemActive() and ImGui::IsItemClicked(0)) {
        toggle();
    }
}

void Checkbox::to_json(nlohmann::json &j) const {
    Element::to_json(j);
    j["type"] = "checkbox";
    j["checked"] = checked;
}

void Checkbox::from_json(const nlohmann::json &j, ImVec2 resolution) {
    if (j.contains("type") && j["type"] != "checkbox") {
        throw std::invalid_argument("Invalid type for Checkbox: expected 'checkbox'");
    }

    Element::from_json(j, resolution);

    if (j.contains("checked") && j["checked"].is_boolean()) {
        checked = j["checked"];
    } else {
        checked = false;
    }
}
std::vector<Setting> Checkbox::getSettings() {
    return {
            {"moduleName",moduleName, [this](const SettingValue& val) { moduleName = std::get<std::string>(val); }},
            {"label", label, [this](const SettingValue& val) { label = std::get<std::string>(val); }}
    };


}