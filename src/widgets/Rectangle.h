#include "Element.h"

class Rectangle : public Element {
private:
    ImVec2 size;
    bool configurationMode;

public:
    // Constructor to initialize position, label, and size
    Rectangle(const std::string& lbl = "", const ImVec2& pos = ImVec2(0.0f, 0.0f), const ImVec2& sz = ImVec2(100.0f, 50.0f),bool configurationMode=false)
            : Element(pos, lbl), size(sz), configurationMode(configurationMode) {}

    ImVec2 getSize() const;
    float getWidth() const;
    float getHeight() const;

    void setSize(const ImVec2& newSize);
    void setWidth(float newWidth);
    void setHeight(float newHeight);

    void draw(ImGuiIO &io) override;
    void handleClicks(ImGuiIO &io) override;
    ImRect getBoundingBox() const override;

    void from_json(const nlohmann::json &j) override;
    void to_json(nlohmann::json &j) const override;
};
