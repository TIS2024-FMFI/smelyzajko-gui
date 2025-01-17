#include "HorizontalSlider.h"

template class HorizontalSlider<int>;
template class HorizontalSlider<float>;

template<typename E>
void HorizontalSlider<E>::draw(ImGuiIO &io) {
    ImGui::SetCursorScreenPos(this->position);
    ImGui::SetNextItemAllowOverlap();
    ImGui::SetNextItemWidth(this->size.x);
    if constexpr (std::is_integral<E>::value) {
        ImGui::SliderInt(this->label.c_str(), (int*)(&this->value), (int)this->minValue, (int)this->maxValue);
    } else {
        ImGui::SliderFloat(this->label.c_str(), &this->value, this->minValue, this->maxValue);
    }

    ImRect bbox = getBoundingBox();
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImVec2 resize_handle_pos = ImVec2(this->position.x + bbox.GetSize().x + 10.0f, this->position.y);
    ImVec2 handle_min = ImVec2(resize_handle_pos.x, resize_handle_pos.y);
    ImVec2 handle_max = ImVec2(resize_handle_pos.x + 10.0f, resize_handle_pos.y + this->size.y);
    draw_list->AddRectFilled(handle_min, handle_max, IM_COL32(255, 0, 0, 255)); // Red resize handle
}

template<typename E>
void HorizontalSlider<E>::handleClicks(ImGuiIO &io) {
    ImRect bbox = getBoundingBox();
    ImVec2 resize_handle_pos = ImVec2(this->position.x + bbox.GetSize().x + 10.0f, this->position.y);
    ImVec2 handle_size = ImVec2(10.0f, 10.0f);

    ImGui::SetCursorScreenPos(resize_handle_pos);
    ImGui::InvisibleButton(("ResizeHandle" + this->label).c_str(), handle_size);

    if (ImGui::IsItemActive() && ImGui::IsMouseDragging(0)) {
        ImVec2 delta = io.MouseDelta;
        this->size.x += delta.x;

        this->size.x = std::max(this->size.x, 100.0f);
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
void HorizontalSlider<E>::to_json(nlohmann::json& j) const {
    std::string type;
    if constexpr (std::is_same_v<E, int>) {
        type = "horizontal-slider-int";
    } else {
        type = "horizontal-slider-float";
    }
    j = nlohmann::json{{"type", type}};
    Slider<E>::to_json(j);
}

template<typename E>
void HorizontalSlider<E>::from_json(const nlohmann::json &j, ImVec2 resolution) {
    std::string type;
    if constexpr (std::is_same_v<E, int>) {
        type = "horizontal-slider-int";
    } else {
        type = "horizontal-slider-float";
    }

    if (j.contains("type") && j["type"] != type) {
        throw std::invalid_argument("Invalid type for HorizontalSlider: expected either horizontal-slider-int or horizontal-slider-float");
    }

    Slider<E>::from_json(j, resolution);

}