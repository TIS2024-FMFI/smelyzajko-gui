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
    void drawElementsWithSnappingOn();
    void setupMenuBar();
    void drawGrid() const;
    void bringElementToTop(Element* element);

    void setupFloatSlider();
    void setupIntSlider();
private:
    float gridSize = 60.0f;
    bool isSnapping = false;
    bool showGrid = false;
    float menuBarHeight;

    const float minGridValue = 10.0f;
    const float maxGridValue = 1000.0f;
};

