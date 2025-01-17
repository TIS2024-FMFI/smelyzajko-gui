#include "Element.h"
#include <string>

class TextInput : public Element {
private:
    ImVec2 size;
    int moduleId = -1;

public:
    TextInput(const std::string& lbl = "", const ImVec2& pos = ImVec2(0.0f, 0.0f), const ImVec2& sz = ImVec2(200.0f, 20.0f))
            : Element(pos, lbl), size(sz) {}

    void draw(ImGuiIO& io) override;
    void handleClicks(ImGuiIO& io) override;
    ImRect getBoundingBox() const override;

    void from_json(const nlohmann::json &j, ImVec2 resolution) override;
    void to_json(nlohmann::json &j) const override;
    std::vector<Setting> getSettings() override;
};
