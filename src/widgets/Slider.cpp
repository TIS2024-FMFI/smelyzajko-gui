#include "Slider.h"
#include "iostream"

template class Slider<float>;
template class Slider<int>;

template <typename E>
E Slider<E>::getValue() const {
    return value;
}

template <typename E>
void Slider<E>::setValue(E val) {
    value = val;
}

template <typename E>
float Slider<E>::getNormalizedValue() const {
    return (value - minValue) / (maxValue - minValue);
}

template <typename E>
void Slider<E>::draw(ImGuiIO& io) {
    ImGui::SetCursorScreenPos(position);
    ImGui::SetNextItemAllowOverlap();
    ImGui::SetNextItemWidth(size.x);
    if constexpr (std::is_integral<E>::value) {
        ImGui::SliderInt(label.c_str(), (int*)(&value), (int)minValue, (int)maxValue);
    } else {
        ImGui::SliderFloat(label.c_str(), &value, minValue, maxValue);
    }

    ImRect bbox = getBoundingBox();
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImVec2 resize_handle_pos = ImVec2(position.x + bbox.GetSize().x + 10.0f, position.y);
    ImVec2 handle_min = ImVec2(resize_handle_pos.x, resize_handle_pos.y);
    ImVec2 handle_max = ImVec2(resize_handle_pos.x + 10.0f, resize_handle_pos.y + size.y);
    draw_list->AddRectFilled(handle_min, handle_max, IM_COL32(255, 0, 0, 255)); // Red resize handle
}

template <typename E>
void Slider<E>::handleClicks(ImGuiIO& io) {
    ImRect bbox = getBoundingBox();
    ImVec2 resize_handle_pos = ImVec2(position.x + bbox.GetSize().x + 10.0f, position.y);
    ImVec2 handle_size = ImVec2(10.0f, 10.0f);

    ImGui::SetCursorScreenPos(resize_handle_pos);
    ImGui::InvisibleButton(("ResizeHandle" + label).c_str(), handle_size);

    if (ImGui::IsItemActive() && ImGui::IsMouseDragging(0)) {
        ImVec2 delta = io.MouseDelta;
        size.x += delta.x;

        size.x = std::max(size.x, 100.0f);
    }

    ImGui::SetCursorScreenPos(position);
    ImGui::InvisibleButton(("Slider" + label).c_str(), bbox.GetSize());

    if (ImGui::IsItemActive() && ImGui::IsMouseDragging(0)) {
        ImVec2 delta = io.MouseDelta;
        position.x += delta.x;
        position.y += delta.y;
    }
}

template <typename E>
ImRect Slider<E>::getBoundingBox() const {
    ImVec2 textSize = ImGui::CalcTextSize(label.c_str());

    float totalWidth = textSize.x + 10.0f;
    float totalHeight = size.y;

    ImVec2 minPos = position;
    ImVec2 maxPos = ImVec2(position.x + totalWidth + size.x, position.y + totalHeight); // Add text width to total width
    return ImRect(minPos, maxPos);
}

template<typename E>
void Slider<E>::to_json(nlohmann::json& j) const {
    j["label"] = label;
    j["position"] = {position.x, position.y};
    j["size"] = {size.x, size.y};
    j["minValue"] = minValue;
    j["maxValue"] = maxValue;
    j["value"] = value;
    j["moduleName"] = moduleName;
}

template<typename E>
void Slider<E>::from_json(const nlohmann::json& j, ImVec2 resolution) {
    Element::from_json(j, resolution);

    if (j.contains("size") && j["size"].is_array() && j["size"].size() == 2) {
        size.x = j["size"][0];
        size.y = j["size"][1];
    } else {
        size = ImVec2(100.0f, 25.0f);
    }

    if (j.contains("minValue")) {
        minValue = j["minValue"].get<E>();
    }
    if (j.contains("maxValue")) {
        maxValue = j["maxValue"].get<E>();
    }
    if (j.contains("value")) {
        value = std::clamp(j["value"].get<E>(), minValue, maxValue);
    }

    if (j.contains("moduleName")) {
        moduleName = j["moduleName"].get<std::string>();
    }

    ImVec2 scale = Element::getScaleFactors(resolution);

    position = ImVec2(position.x * scale.x, position.y * scale.y);
    size = ImVec2(size.x * scale.x, size.y * scale.y);
}

template<typename E>
std::vector<Setting> Slider<E>::getSettings() {
    return {
            {"moduleName",moduleName, [this](const SettingValue& val) { moduleName = std::get<std::string>(val); }},
            {"label", label, [this](const SettingValue& val) { label = std::get<std::string>(val); }},
            {"minValue", minValue, [this](const SettingValue& val) { minValue = std::get<E>(val); }},
            {"maxValue", maxValue, [this](const SettingValue& val) { maxValue = std::get<E>(val); }}
    };
}