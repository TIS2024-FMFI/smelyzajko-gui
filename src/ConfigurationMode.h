#include "GUI.h"
#include "TemplateHandler.h"
#include <algorithm>

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

private:
    float gridSize = 60.0f;
    bool isSnapping = false;
    bool showGrid = false;
};


