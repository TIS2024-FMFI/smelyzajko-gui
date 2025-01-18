#include "Element.h"


class Rectangle : public Element {
private:

    int moduleId = -1;

    std::string graphicElementName;
    int graphicElementId;

    int graphicsFrequency = 0.0;
    bool graphicsLogEnabled= false;
    int textFrequency =  0.0;
    bool textLogEnabled= false;



public:
    // Constructor to initialize position, label, and size
    Rectangle(const std::string& lbl = "", const ImVec2& pos = ImVec2(0.0f, 0.0f), const ImVec2& sz = ImVec2(100.0f, 50.0f))
            : Element(lbl, "", pos, sz)  {}

    float getWidth() const;
    float getHeight() const;

    void setWidth(float newWidth);
    void setHeight(float newHeight);

    void setModuleID(int id);
    void setGraphicElementId(int id);
    void setModuleName(const std::string& name) override;
    void setGraphicElementName(std::string name);

    void draw(ImGuiIO &io) override;
    void handleClicks(ImGuiIO &io) override;
    ImRect getBoundingBox() const override;

    void from_json(const nlohmann::json &j, ImVec2 resolution) override;
    void to_json(nlohmann::json &j) const override;
    std::vector<Setting> getSettings() override;

};
