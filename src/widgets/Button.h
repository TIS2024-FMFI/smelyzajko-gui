#include "Element.h"

class Button : public Element {
private:
    bool clicked = false;
public:
    Button(const std::string& lbl = "", const std::string& moduleName = "", const ImVec2& pos = ImVec2(0.0f, 0.0f), const ImVec2& sz = ImVec2(100.0f, 25.0f))
            : Element(lbl, moduleName, pos, sz) {
}
    std::optional<std::string > getStringValue()  override;


    ImRect getBoundingBox() const override;
    void draw(ImGuiIO& io) override;
    void handleClicks(ImGuiIO &io) override;
    static void setStyles();
    static void removeStyles();

    void to_json(nlohmann::json &j) const override;
    void from_json(const nlohmann::json &j, ImVec2 resolution) override;
    std::vector<Setting> getSettings()  override;
};
