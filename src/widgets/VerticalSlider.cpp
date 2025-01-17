#include "VerticalSlider.h"

template class VerticalSlider<int>;
template class VerticalSlider<float>;

template<typename E>
void VerticalSlider<E>::draw(ImGuiIO &io) {
    ImGui::SetCursorScreenPos(this->position);
    ImGui::SetNextItemAllowOverlap();
    ImGui::SetNextItemWidth(this->size.x);

    if constexpr (std::is_integral<E>::value) {
        ImGui::VSliderInt(this->label.c_str(), ImVec2(this->size.x, this->size.y), (int*)&this->value, (int)this->minValue, (int)this->maxValue);
    } else {
        ImGui::VSliderFloat(this->label.c_str(), ImVec2(this->size.x, this->size.y), &this->value, this->minValue, this->maxValue);
    }

    ImRect bbox = getBoundingBox();
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImVec2 resize_handle_pos = ImVec2(this->position.x, this->position.y + bbox.GetSize().y + 10.0f);
    ImVec2 handle_min = ImVec2(resize_handle_pos.x, resize_handle_pos.y);
    ImVec2 handle_max = ImVec2(resize_handle_pos.x + this->size.x, resize_handle_pos.y + 10);
    draw_list->AddRectFilled(handle_min, handle_max, IM_COL32(255, 0, 0, 255)); // Red resize handle
}

template<typename E>
void VerticalSlider<E>::handleClicks(ImGuiIO &io) {
    ImRect bbox = getBoundingBox();

    ImVec2 resize_handle_pos = ImVec2(this->position.x, this->position.y + bbox.GetSize().y + 10.0f);
    ImVec2 handle_size = ImVec2(this->size.x, 10.0f);

    ImGui::SetCursorScreenPos(resize_handle_pos);
    ImGui::InvisibleButton(("ResizeHandle" + this->label).c_str(), handle_size);

    if (ImGui::IsItemActive() && ImGui::IsMouseDragging(0)) {
        ImVec2 delta = io.MouseDelta;

        this->size.y += delta.y;

        this->size.y = std::max(this->size.y, 100.0f);
    }

    ImGui::SetCursorScreenPos(this->position);
    ImGui::InvisibleButton(("Slider" + this->label).c_str(), bbox.GetSize());

    if (ImGui::IsItemActive() && ImGui::IsMouseDragging(0)) {
        ImVec2 delta = io.MouseDelta;
        this->position.x += delta.x;
        this->position.y += delta.y;
    }
}

template<typename E>
void VerticalSlider<E>::to_json(nlohmann::json& j) const {
    std::string type;
    if constexpr (std::is_same_v<E, int>) {
        type = "vertical-slider-int";
    } else {
        type = "vertical-slider-float";
    }
    j = nlohmann::json{{"type", type}};
    Slider<E>::to_json(j);
}

template<typename E>
void VerticalSlider<E>::from_json(const nlohmann::json &j, ImVec2 resolution) {
    std::string type;
    if constexpr (std::is_same_v<E, int>) {
        type = "vertical-slider-int";
    } else {
        type = "vertical-slider-float";
    }

    if (j.contains("type") && j["type"] != type) {
        throw std::invalid_argument("Invalid type for VerticalSlider: expected either vertical-slider-int or vertical-slider-float");
    }

    Slider<E>::from_json(j, resolution);
}
