#include "GUI.h"
#include "TemplateHandler.h"
#include <algorithm>
#include "Module.h"

class ConfigurationMode : GUI {
public:

    ConfigurationMode() : io(ImGui::GetIO()) {}

    std::vector<Element *> activeElements;
    TemplateHandler templateHandler;
    ImGuiIO& io;

    int run() override;

    void drawElements();
    void setupMenuBar();
    void drawGrid() const;
    void bringElementToTop(Element* element);
    void renderSettingsPopup(Module& module, const std::string& part);

    void setupFloatSlider();
    void setupIntSlider();
private:
    float gridSize = 60.0f;
    bool isSnapping = false;
    bool showGrid = false;
};


