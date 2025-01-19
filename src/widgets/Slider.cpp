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
    Element::to_json(j);
    j["minValue"] = minValue;
    j["maxValue"] = maxValue;
    j["value"] = value;
}

template<typename E>
void Slider<E>::from_json(const nlohmann::json& j, ImVec2 resolution) {
    Element::from_json(j, resolution);

    if (j.contains("minValue")) {
        minValue = j["minValue"].get<E>();
    }
    if (j.contains("maxValue")) {
        maxValue = j["maxValue"].get<E>();
    }
    if (j.contains("value")) {
        value = std::clamp(j["value"].get<E>(), minValue, maxValue);
    }
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