#include "Element.h"

class Checkbox : public Element {
private:
    bool checked;

public:
    Checkbox(const std::string& lbl = "", const ImVec2& pos = ImVec2(0.0f, 0.0f), bool initialState = false)
            : Element(pos, lbl), checked(initialState) {}

    bool isChecked() const;
    void setChecked(bool state);
    void toggle();

    ImRect getBoundingBox() const override;

    void draw(ImGuiIO &io) override;
    void handleClicks(ImGuiIO &io) override;

    void to_json(nlohmann::json &j) const override;
    void from_json(const nlohmann::json &j, ImVec2 resolution) override;
};