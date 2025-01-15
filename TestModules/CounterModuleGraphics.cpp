#include "CounterModuleGraphics.h"

void CounterModuleGraphics::draw(ImGuiIO &io) {

    ImDrawList* draw_list = ImGui::GetForegroundDrawList();

    // Text content
    std::string text = "Counter: " + std::to_string(counter);

    // Define the color of the text (white) and border
    ImU32 text_color = IM_COL32(255, 255, 255, 255);
    ImU32 border_color = IM_COL32(255, 255, 255, 255); // White border

    const char* text_cstr = text.c_str();
    ImVec2 text_size = ImGui::CalcTextSize(text_cstr);

    ImVec2 rect_min = position;
    ImVec2 rect_max = ImVec2(position.x + size.x, position.y + size.y);
    const float border_thickness = 2.0f;

    draw_list->AddRect(rect_min, rect_max, border_color, 0.0f, 0, border_thickness);
    ImVec2 text_pos = ImVec2((rect_min.x + rect_max.x) / 2.0f - text_size.x / 2.0f, (rect_min.y + rect_max.y) / 2.0f - text_size.y / 2.0f);

    draw_list->AddText(text_pos, text_color, text_cstr);
}

void CounterModuleGraphics::updateValueOfModule(int value) {
    counter = value;
}

void CounterModuleGraphics::updateValueOfModule(std::string value) {
    std::cerr << "Invalid value for MapModuleGraphics." << std::endl;
}

void CounterModuleGraphics::updateValueOfModule(std::vector<int> value) {
    std::cerr << "Invalid value for MapModuleGraphics." << std::endl;

}

void CounterModuleGraphics::from_json(const nlohmann::json &j, ImVec2 resolution) {
    if (j.contains("graphicModuleId") && j["graphicModuleId"].is_number_integer()) {
        graphicModuleId = j["graphicModuleId"];
    }
    if (j.contains("moduleId") && j["moduleId"].is_number_integer()) {
        moduleId = j["moduleId"];
    }
    if (j.contains("position") && j["position"].is_array() && j["position"].size() == 2) {
        position.x = j["position"][0];
        position.y = j["position"][1];
        setPos({j["position"][0], j["position"][1]});
    }
    if (j.contains("size") && j["size"].is_array() && j["size"].size() == 2) {
        size.x = j["size"][0];
        size.y = j["size"][1];
        setSize({j["size"][0], j["size"][1]});
    }
    if (j.contains("graphicsFrequency") && j["graphicsFrequency"].is_number_float()) {
        graphicsFrequency = j["graphicsFrequency"];
    }
    if (j.contains("graphicsLogEnabled") && j["graphicsLogEnabled"].is_boolean()) {
        graphicsLogEnabled = j["graphicsLogEnabled"];
    }
    if (j.contains("textFrequency") && j["textFrequency"].is_number_float()) {
        textFrequency = j["textFrequency"];
    }
    if (j.contains("textLogEnabled") && j["textLogEnabled"].is_boolean()) {
        textLogEnabled = j["textLogEnabled"];
    }

    GraphicModule::scaleFromResolution(resolution);
}
void CounterModuleGraphics::to_json(nlohmann::json &j) const {
    j["graphicModuleId"] = graphicModuleId;
    j["moduleId"] = moduleId;
    j["name"] = moduleName;
    j["position"] = {position.x, position.y};
    j["size"] = {size.x, size.y};
    j["graphicsFrequency"] = graphicsFrequency;
    j["graphicsLogEnabled"] = graphicsLogEnabled;
    j["textFrequency"] = textFrequency;
    j["textLogEnabled"] = textLogEnabled;
}


